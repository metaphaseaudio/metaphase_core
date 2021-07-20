//
// Created by Matt on 7/7/2021.
//

#pragma once
#include <juce_dsp/juce_dsp.h>

namespace meta
{
namespace dsp
{
    class MultiChanConvolve
    {
    public:
        MultiChanConvolve(juce::AudioBuffer<float>&& impulse, size_t max_block_size)
            : m_Spec{ 44100, static_cast<juce::uint32>(max_block_size), 1 }
            , m_IRLength(impulse.getNumSamples())
            , m_Conv(impulse.getNumChannels())
            , m_Tmp(1, m_IRLength + max_block_size)
        {
            for (auto c = impulse.getNumChannels(); --c >= 0;)
            {
                juce::AudioBuffer<float> tmp(1, m_IRLength);
                tmp.copyFrom(0, 0, impulse, c, 0, m_IRLength);
                m_Conv.at(c).loadImpulseResponse(
                        std::move(tmp), 44100,
                        juce::dsp::Convolution::Stereo::no,
                        juce::dsp::Convolution::Trim::no,
                        juce::dsp::Convolution::Normalise::no);
                m_Conv.at(c).prepare(m_Spec);
            }
        }

        juce::AudioBuffer<float> convolve(const juce::AudioBuffer<float>& in, bool include_tail=true)
        {
            jassert(in.getNumChannels() == m_Conv.size());

            auto total_length = in.getNumSamples() + (include_tail ? m_IRLength : 0);
            juce::AudioBuffer<float> rv(m_Conv.size(), total_length);
            juce::AudioBuffer<float> extended_in;
            extended_in.makeCopyOf(in);
            extended_in.setSize(in.getNumChannels(), total_length, true, true);
            rv.clear();
            juce::AudioBuffer<float> tmp_in, tmp_out;

            for (int mod = 0; mod < total_length; mod += m_Spec.maximumBlockSize)
            {
                const auto proc_len = std::min<int>(m_Spec.maximumBlockSize, total_length - mod);
                tmp_in.setDataToReferTo(extended_in.getArrayOfWritePointers(), in.getNumChannels(), mod, proc_len);
                tmp_out.setDataToReferTo(rv.getArrayOfWritePointers(), rv.getNumChannels(), mod, proc_len);
                process_block(tmp_in, tmp_out);
            }

            return rv;
        }

        void process_block(juce::AudioBuffer<float>& in, juce::AudioBuffer<float>& out)
        {
            jassert(in.getNumSamples() <= m_Spec.maximumBlockSize && out.getNumSamples() <= m_Spec.maximumBlockSize);

            for (auto chan = out.getNumChannels(); --chan >= 0;)
            {
                // Clear temp
                m_Tmp.clear();

                // Convolve
                juce::dsp::AudioBlock<float> in_block(in.getArrayOfWritePointers() + chan, 1, 0, in.getNumSamples());
                juce::dsp::AudioBlock<float> out_block(m_Tmp);
                juce::dsp::ProcessContextNonReplacing<float> context(in_block, out_block);
                m_Conv.at(chan).process(context);

                // Move to output, trimmed
                auto offset = 0;
                out.copyFrom(chan, 0, m_Tmp, 0, offset, in.getNumSamples());
            }
        }

        int get_ir_length() const { return m_IRLength; }
    private:
        int m_IRLength;
        juce::dsp::ProcessSpec m_Spec;
        juce::AudioBuffer<float> m_Tmp;
        std::vector<juce::dsp::Convolution> m_Conv;
    };
}
}
