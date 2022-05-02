//
// Created by Matt on 4/23/2022.
//

#pragma once
#include <meta/util/NumericConstants.h>
#include <meta/util/math.h>
#include <meta/audio/Blip_Buffer.h>
#include <juce_audio_basics/juce_audio_basics.h>


namespace meta
{
    template<size_t bit_depth, size_t sub_samples, size_t blip_resolution=8, size_t chans=1>
    class OversampledBuffer
    {
    public:
        static constexpr int Min = meta::BitInfo<bit_depth, true>::Min;
        static constexpr unsigned int Max = meta::BitInfo<bit_depth, true>::Max;
        static constexpr unsigned long Range = meta::abs(Max - Min);
        static constexpr size_t OverSample = sub_samples;

        explicit OversampledBuffer(long sample_rate)
        {
            set_sample_rate(sample_rate);
            for (int i = chans; --i >=0;)
            {
                m_BlipBuffs[i].clock_rate(m_BlipBuffs[i].sample_rate() * sub_samples);
                m_BlipBuffs[i].bass_freq(0); // makes waveforms perfectly flat
                m_BlipSynths[i].volume(1.0);
                m_BlipSynths[i].output(&m_BlipBuffs[i]);
            }
        }

        void set_sample_rate(long sample_rate)
        {
            // use a second's worth of buffer, it's plenty.
            for (int i = chans; --i >=0;)
            { m_BlipBuffs[i].set_sample_rate(sample_rate, 1000); }
        }

        void downsampleBuffer(const juce::AudioBuffer<float>& inData, juce::AudioBuffer<float>& outBuffer)
        {
            jassert(inData.getNumSamples() == outBuffer.getNumSamples() * OverSample);
            jassert(inData.getNumChannels() == outBuffer.getNumChannels());

            juce::AudioBuffer<float> tmpData = inData;
            tmpData.applyGain(-Min);

            for (int c = 0; c < chans; c++)
            {
                for (int s = 0; s < inData.getNumSamples(); s++)
                    { m_BlipSynths[c].update(s, tmpData.getSample(c, s)); }
                m_BlipBuffs[c].end_frame(inData.getNumSamples());
                relocate_samples(m_BlipBuffs[c], outBuffer.getWritePointer(c), outBuffer.getNumSamples());
            }
        }


    private:
        long relocate_samples(Blip_Buffer& buff, float* out, long out_size, float low = -1.0f, float high = 1.0f)
        {
            // Limit number of samples read to those available
            long count = buff.samples_avail();
            if (count > out_size)
                count = out_size;

            // Begin reading samples from Blip_Buffer
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

        Blip_Reader reader;
        std::array<Blip_Synth<blip_resolution, Range>, chans> m_BlipSynths;
        std::array<Blip_Buffer, chans> m_BlipBuffs;
    };
}
