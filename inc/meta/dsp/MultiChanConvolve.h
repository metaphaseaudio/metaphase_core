//
// Created by Matt on 7/7/2021.
//

#pragma once
#include <JuceHeader.h>

namespace meta
{
namespace dsp
{
    class MultiChanConvolve
    {
    public:
        MultiChanConvolve(juce::AudioBuffer<float>&& impulse, size_t max_block_size)
            : m_Spec{ 1, static_cast<juce::uint32>(max_block_size), 1 }
            , m_IRLength(impulse.getNumSamples())
            , m_Conv(m_IRLength)
        {
            for (auto c = impulse.getNumChannels(); --c >= 0;)
            {

                juce::AudioBuffer<float> tmp(1, m_IRLength);
                tmp.copyFrom(0, 0, impulse, c, 0, m_IRLength);
                m_Conv.at(c).loadImpulseResponse(
                        std::move(tmp), 1,
                        juce::dsp::Convolution::Stereo::no,
                        juce::dsp::Convolution::Trim::no,
                        juce::dsp::Convolution::Normalise::no);
                m_Conv.at(c).prepare(m_Spec);
            }
        }

        void process_block(juce::AudioBuffer<float>& in, juce::AudioBuffer<float>& out)
        {
            const auto total_length = in.getNumSamples() + m_IRLength;
            auto tmp = juce::AudioBuffer<float>(1, total_length);

            for (auto chan = out.getNumChannels(); --chan >= 0;)
            {
                // Clear temp
                tmp.clear();

                // Convolve
                juce::dsp::AudioBlock<float> in_block(in.getArrayOfWritePointers() + chan, 1, 0, in.getNumSamples());
                juce::dsp::AudioBlock<float> out_block(tmp);
                juce::dsp::ProcessContextNonReplacing<float> context(in_block, out_block);
                m_Conv.at(chan).process(context);

                // Move to output, trimmed
                auto offset = 0;
                out.copyFrom(chan, 0, tmp, 0, offset, total_length);
            }
        }

    private:
        juce::dsp::ProcessSpec m_Spec;
        int m_IRLength;
        std::vector<juce::dsp::Convolution> m_Conv;
    };
}
}
