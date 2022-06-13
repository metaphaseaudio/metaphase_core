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
    template<size_t bit_depth, size_t sub_samples, size_t blip_resolution=8>
    class OversampledBuffer
    {
    public:
        static constexpr int Min = meta::BitInfo<bit_depth, true>::Min;
        static constexpr unsigned int Max = meta::BitInfo<bit_depth, true>::Max;
        static constexpr unsigned long Range = meta::abs(Max - Min);
        static constexpr size_t OverSample = sub_samples;

        explicit OversampledBuffer(int sample_rate, int chans)
            : m_Chans(chans)
        {
            for (int i = 0; i < chans; i++)
            {
                m_BlipBuffs.emplace_back(new Blip_Buffer());
                m_BlipBuffs[i]->bass_freq(0); // makes waveforms perfectly flat

                m_BlipSynths.emplace_back(new Blip_Synth<blip_resolution, Range>());
                m_BlipSynths[i]->volume(1.0);
                m_BlipSynths[i]->output(m_BlipBuffs[i].get());
            }

            set_sample_rate(sample_rate);
        }

        void set_sample_rate(int sample_rate)
        {
            // use a second's worth of buffer, it's plenty.
            for (int i = m_Chans; --i >=0;)
            {
                m_BlipBuffs[i]->set_sample_rate(sample_rate, 1000);
                m_BlipBuffs[i]->clock_rate(m_BlipBuffs[i]->sample_rate() * sub_samples);
            }
        }

        void downsampleBuffer(const juce::AudioBuffer<float>& inData, juce::AudioBuffer<float>& outBuffer)
        {
            jassert(inData.getNumSamples() >= outBuffer.getNumSamples() * OverSample);
            jassert(inData.getNumChannels() == outBuffer.getNumChannels());
            jassert(inData.getNumChannels() <= m_Chans);

            juce::AudioBuffer<float> tmpData = inData;
            tmpData.applyGain(-Min);

            for (int c = 0; c < inData.getNumChannels(); c++)
            {
                auto offset = 0;
                auto samps = outBuffer.getNumSamples() * OverSample;

                while (samps > 0)
                {
                    const auto samps_available = (m_BlipBuffs[c]->sample_rate() - m_BlipBuffs[c]->samples_avail()) * OverSample;
                    const auto to_render = std::min<int>(samps, samps_available);
                    const auto resampled_offset = offset / OverSample;

                    for (int s = 0; s < to_render; s++)
                        { m_BlipSynths[c]->update(s, tmpData.getSample(c, s + offset)); }

                    m_BlipBuffs[c]->end_frame(to_render);

                    relocate_samples(
                        *m_BlipBuffs[c],
                        outBuffer.getWritePointer(c) + resampled_offset,
                        outBuffer.getNumSamples() - resampled_offset
                    );
                    offset += to_render;
                    samps -= to_render;
                }
            }
        }

        // Non-copyable.
        OversampledBuffer(const OversampledBuffer&) = delete;
        OversampledBuffer& operator=(const OversampledBuffer&) = delete;
    private:

        int relocate_samples(Blip_Buffer& buff, float* out, int out_size, float low = -1.0f, float high = 1.0f)
        {
            // Limit number of samples read to those available
            long count = buff.samples_avail();
            if (count > out_size)
                count = out_size;

            // Begin reading samples from Blip_Buffer

            Blip_Reader reader;
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

        int m_Chans;
        std::vector<std::unique_ptr<Blip_Synth<blip_resolution, Range>>> m_BlipSynths;
        std::vector<std::unique_ptr<Blip_Buffer>> m_BlipBuffs;
    };
}
