//
// Created by Matt Zapp on 3/15/2020.
//

#pragma once

#include <meta/util/testing/TestBase.h>
#include <meta/util/testing/TestHelpers.h>
#include <meta/util/file/AudioFileHelpers.h>
#include <meta/audio/Blip_Buffer.h>


#define BLIP_OSC_SAMPLE_RATE 48000

template <size_t bit_depth, size_t sub_samples, size_t blip_resolution=8>
class BandLimitedOsc
{
    static constexpr int Min = meta::BitInfo<bit_depth, true>::Min;
    static constexpr unsigned int Max = meta::BitInfo<bit_depth, true>::Max;
    static constexpr unsigned long range = Max - Min;
public:
    explicit BandLimitedOsc(long sample_rate)
        : accum(0)
        , delta(0)
        , time_i(0)
    {
        std::cout << Min << " " << Max << range << std::endl;
        m_BlipBuff.set_sample_rate(sample_rate, 1000); // Just use a second's worth of buffer, it's plenty.
        m_BlipBuff.clock_rate(m_BlipBuff.sample_rate() * sub_samples);
        m_BlipBuff.bass_freq(0); // makes waveforms perfectly flat
        synth.volume(0.75);
        synth.output(&m_BlipBuff);
    }

    void set_freq(float freq) {
        // At least two samples are required to represent nyquist. Higher than this shall not pass.
        const auto sub_samples_per_cycle = std::max(2.0f * sub_samples, m_BlipBuff.clock_rate() / freq);
        const float frange = range;
        delta = frange / sub_samples_per_cycle;
    }

    void sync(float value) { accum = std::max<float>(std::min<float>(value, Max), Min + 1); }
    
    float tick_delta()
    {
        return delta;
    }
    
    float wave_shape(float accumulator_state)
    {
//        return accumulator_state;
        return accumulator_state > 0 ? meta::NumInfo<short>::Max : meta::NumInfo<short>::Min;
//        return std::abs(accumulator_state) * 2 - Max;
    }

    void processBlock(float* block, long n_samps)
    {
        for (long i = n_samps * sub_samples; --i >= 0;)
        {
            if (tick()) {
                const auto to_relocate = std::min(m_BlipBuff.samples_avail(), n_samps);
                relocate_samples(block, to_relocate);
                n_samps -= to_relocate;
            }
        }
        end_block();
        relocate_samples(block, n_samps);
    }

private:
    bool tick()
    {
        const auto next = std::floor(wave_shape(accum));
        synth.update(time_i, next);
        accum += tick_delta();
        time_i++;

        if (std::floor(accum) > Max)
            { accum -= range; }
        if (time_i >= m_BlipBuff.clock_rate())
        {
            end_block();
            return true;
        }

        return false;
    }

    void end_block()
    {
        m_BlipBuff.end_frame(time_i);
        time_i = 0;
    }

    long relocate_samples(float* out, long out_size, float low = -1.0f, float high = 1.0f )
    {
        // Limit number of samples read to those available
        long count = m_BlipBuff.samples_avail();
        if ( count > out_size )
            count = out_size;

        // Begin reading samples from Blip_Buffer
        auto reader = Blip_Reader();
        int bass = reader.begin(m_BlipBuff);

        float factor = (high - low) * 0.5f;
        float offset = low + 1.0f * factor;
        unsigned long const sample_range = 1UL << blip_sample_bits;
        factor *= 1.0f / float(sample_range / 2);

        for ( long n = count; n--; )
        {
            // Read sample at full resolution and convert to output format
            long s = reader.read_raw();
            reader.next( bass );
            *out++ = s * factor + offset;
            if ((unsigned long) (s + sample_range / 2) >= sample_range)
            {
                // clamp
                out[-1] = high;
                if ( s < 0 )
                    out[-1] = low;
            }
        }

        // End reading and remove the samples
        reader.end(m_BlipBuff);
        m_BlipBuff.remove_samples( count );

        return count;
    }

    float accum;
    float delta;
    int time_i;
    Blip_Buffer m_BlipBuff;
    Blip_Synth<blip_resolution, range> synth;
};


class BlipSynthTest
    : public meta::TestBase
{
public:
    BlipSynthTest()
        : osc(BLIP_OSC_SAMPLE_RATE)
    {};


    void initializeTestFile(const juce::String& f)
    {
        meta::TestBase::initializeTestFile(meta::TestHelpers::testFolder.getChildFile(f));
    }

    void runOscillator(int samples)
    {
        constexpr int chunk_size = 528;
        for (int mod = samples / chunk_size; --mod >= 0;)
        {
            const auto n_samps = std::min<int>(chunk_size, samples);

            juce::AudioBuffer<float> buffer(1, n_samps);
            buffer.clear();
            osc.processBlock(buffer.getWritePointer(0), n_samps);
            m_Writer->writeFromAudioSampleBuffer(buffer, 0, buffer.getNumSamples());
            samples -= chunk_size;
        }

    }

    BandLimitedOsc<16, 64, 12> osc;
};


TEST_F(BlipSynthTest, generate)
{
    initializeTestFile("blip_osc2.wav");
//    osc.set_freq(125 * 16);

    osc.set_freq(125);
    float n_seconds = 2;
    runOscillator(BLIP_OSC_SAMPLE_RATE * n_seconds);
//    osc.set_freq(250);
//    runOscillator(BLIP_OSC_SAMPLE_RATE * n_seconds);
}
