//
// Created by Matt on 8/15/2025.
//

#pragma once

#include <functional>
#include "juce_audio_basics/juce_audio_basics.h"

namespace meta
{
    template <typename FloatType>
    class BufferAccumulator
    {
    public:
        BufferAccumulator(int nChans, int targetSizeSamps)
            : m_Buffer(nChans, targetSizeSamps)
            , m_SamplesRemaining(targetSizeSamps)
        {};

        std::function<void(juce::AudioBuffer<FloatType>&)> onComplete;

        void accumulate(const juce::AudioBuffer<FloatType>& x)
        {
            jassert(x.getNumChannels() == m_Buffer.getNumChannels());
            auto inSamplesRemaining = x.getNumSamples();

            const auto recalculateNToCopy = [&](){
                return std::min<int>(m_SamplesRemaining, inSamplesRemaining);
            };

            auto nToCopy = recalculateNToCopy();

            while (nToCopy > 0)
            {
                auto s = m_Buffer.getNumSamples() - m_SamplesRemaining;
                for (int c = 0; c < x.getNumChannels(); c++)
                {
                    m_Buffer.copyFrom(0, s, x, c, x.getNumSamples() - inSamplesRemaining, nToCopy);
                }

                m_SamplesRemaining -= nToCopy;
                inSamplesRemaining -= nToCopy;

                if (m_SamplesRemaining == 0)
                {
                    onComplete(m_Buffer);
                    m_SamplesRemaining = m_Buffer.getNumSamples();
                }

                nToCopy = recalculateNToCopy();
            }
        }

    private:
        int m_SamplesRemaining{0};
        juce::AudioBuffer<FloatType> m_Buffer;
    };

}