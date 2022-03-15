//
// Created by Matt on 5/16/2021.
//
#pragma once
#include <meta/audio/Blip_Buffer.h>
#include <meta/audio/LoopingAccumulator.h>
#include <meta/util/NumericConstants.h>

namespace meta
{
    template<size_t bit_depth, size_t sub_samples, size_t blip_resolution = 8>
    class BandLimitedOsc
    {
    public:
        static constexpr int Min = meta::BitInfo<bit_depth, true>::Min;
        static constexpr unsigned int Max = meta::BitInfo<bit_depth, true>::Max;
        static constexpr unsigned long Range = meta::abs(Max - Min);

        explicit BandLimitedOsc(long sample_rate)
            : accumulator{sample_rate * sub_samples, 0 , 0}, clock_i(0)
        {
            set_sample_rate(sample_rate);
            m_BlipBuff.clock_rate(m_BlipBuff.sample_rate() * sub_samples);
            m_BlipBuff.bass_freq(0); // makes waveforms perfectly flat
            synth.volume(0.8);
            synth.output(&m_BlipBuff);
        }

        void set_sample_rate(long sample_rate)
        {
            // use a second's worth of buffer, it's plenty.
            m_BlipBuff.set_sample_rate(sample_rate, 1000);
        }

        void set_freq(float freq) { accumulator.set_freq(freq); }

        void sync(float value) { accumulator.sync(value); }

        /**
         * Override this to set up a different waveform. returns the accumulator
         * state by default
         * @return the next value of the wave to produce
         */
        virtual float wave_shape(float accumulator_state) { return accumulator_state; }

        void processBlock(float* block, long n_samps)
        {
            size_t offset = 0;
            while (n_samps > 0)
            {
                const auto block_size = std::min(m_BlipBuff.sample_rate() - m_BlipBuff.samples_avail(), n_samps);
                tick(block_size);
                relocate_samples(block + offset, block_size);
                n_samps -= block_size;
                offset += block_size;
            }
        }

        void tick(int sample_count = 1)
        {
            assert(sample_count <= m_BlipBuff.sample_rate() - m_BlipBuff.samples_avail());
            const auto clock_count = sample_count * sub_samples;
            for (int i = clock_count; --i >= 0; clock_i++)
            {
                const auto next = std::floor(wave_shape(accumulator.value));
                synth.update(clock_i, next);
                accumulator.tick();
            }
            end_block();
        }


    protected:
        void end_block()
        {
            m_BlipBuff.end_frame(clock_i);
            clock_i = 0;
        }

        long relocate_samples(float* out, long out_size, float low = -1.0f, float high = 1.0f)
        {
            // Limit number of samples read to those available
            long count = m_BlipBuff.samples_avail();
            if (count > out_size)
                count = out_size;

            // Begin reading samples from Blip_Buffer
            auto reader = Blip_Reader();
            int bass = reader.begin(m_BlipBuff);

            float factor = (high - low) * 0.5f;
            float offset = low + 1.0f * factor;
            unsigned long const sample_range = 1UL << blip_sample_bits;
            factor *= 1.0f / float(sample_range / 2);

            for (long n = count; n--;)
            {
                // Read sample at full resolution and convert to output format
                long s = reader.read_raw();
                reader.next(bass);
                *out++ = s * factor + offset;
                if ((unsigned long) (s + sample_range / 2) >= sample_range)
                {
                    // clamp
                    out[-1] = high;
                    if (s < 0)
                        out[-1] = low;
                }
            }

            // End reading and remove the samples
            reader.end(m_BlipBuff);
            m_BlipBuff.remove_samples(count);

            return count;
        }

        LoopingAccumulator<Min, Max> accumulator;
        int clock_i;
        Blip_Buffer m_BlipBuff;
        Blip_Synth<blip_resolution, Range> synth;
    };
}