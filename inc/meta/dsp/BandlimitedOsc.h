//
// Created by Matt on 5/16/2021.
//
#pragma once
#include <meta/audio/Blip_Buffer.h>
#include <meta/audio/LoopingAccumulator.h>
#include <meta/util/NumericConstants.h>
#include <meta/util/container_helpers/array.h>

namespace meta
{
    template<size_t bit_depth, size_t sub_samples, size_t blip_resolution=8, size_t chans=1>
    class BandLimitedOsc
    {
    public:
        static constexpr int Min = meta::BitInfo<bit_depth, true>::Min;
        static constexpr unsigned int Max = meta::BitInfo<bit_depth, true>::Max;
        static constexpr unsigned long Range = meta::abs(Max - Min);
        static constexpr size_t Latency = blip_resolution;
        static constexpr size_t OverSample = sub_samples;

        explicit BandLimitedOsc(long sample_rate)
            : accumulator(Min, Max, sample_rate * sub_samples), clock_i(0)
        {
            set_sample_rate(sample_rate);
            for (int i = chans; --i >=0;)
            {
                m_BlipBuffs[i].clock_rate(m_BlipBuffs[i].sample_rate() * sub_samples);
                m_BlipBuffs[i].bass_freq(0); // makes waveforms perfectly flat
                synths[i].volume(0.8);
                synths[i].output(&m_BlipBuffs[i]);
            }
        }

        void set_sample_rate(long sample_rate)
        {
            // use a second's worth of buffer, it's plenty.
            for (int i = chans; --i >=0;)
                { m_BlipBuffs[i].set_sample_rate(sample_rate, 1000); }
        }

        void set_freq(float freq)
            { accumulator.set_freq(freq); }

        void sync(float value) { accumulator.sync(value); }

        /**
         * Override this to set up a different waveform. returns the accumulator
         * state by default
         * @return the next value of the wave to produce
         */
        virtual float wave_shape(float accumulator_state, int chan) { return accumulator_state; }

        void processBlock(float** block, long n_samps)
        {
            size_t offset = 0;
            while (n_samps > 0)
            {
                const auto block_size = std::min(m_BlipBuffs[0].sample_rate() - m_BlipBuffs[0].samples_avail(), n_samps);
                tick(block_size);
                for (int c = chans; --c >= 0;)
                    { relocate_samples(block[c] + offset, block_size, c); }
                n_samps -= block_size;
                offset += block_size;
            }
        }

        void tick(int sample_count = 1)
        {
            assert(sample_count <= m_BlipBuffs[0].sample_rate() - m_BlipBuffs[0].samples_avail());
            const auto n_clock = sample_count * sub_samples;
            for (; clock_i < n_clock; clock_i++)
            {
                if (! (clock_i % sub_samples))
                    { onSample(accumulator.getValue()); }

                for (const auto chan_value : meta::enumerate(onTick(accumulator.tick())))
                    { synths.at(std::get<0>(chan_value)).update(clock_i, std::get<1>(chan_value)); }
            }
            end_block();
        }

    protected:
        virtual void onSample(float accum_state) {};
        virtual std::array<float, chans> onTick(float accum_state) {
            return meta::make_initialized_array<float, chans>(accum_state);
        };

        void end_block()
        {
            for (auto& buff : m_BlipBuffs)
                { buff.end_frame(clock_i); }

            clock_i = 0;
        }

        long relocate_samples(float* out, long out_size, int chan = 0, float low = -1.0f, float high = 1.0f)
        {
            Blip_Buffer& buff = m_BlipBuffs[chan];
            // Limit number of samples read to those available
            long count = buff.samples_avail();
            if (count > out_size)
                count = out_size;

            // Begin reading samples from Blip_Buffer
            auto reader = Blip_Reader();
            int bass = reader.begin(buff);

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

                // Clamp the values
                if ((unsigned long) (s + sample_range / 2) >= sample_range)
                {
                    out[-1] = high;
                    if (s < 0) { out[-1] = low; }
                }
            }

            // End reading and remove the samples
            reader.end(buff);
            buff.remove_samples(count);

            return count;
        }

        LoopingAccumulator accumulator;
        int clock_i;
        std::array<Blip_Buffer, chans> m_BlipBuffs;
        std::array<Blip_Synth<blip_resolution, Range>, chans> synths;
    };
}