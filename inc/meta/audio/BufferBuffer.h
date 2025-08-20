//
// Created by Matt on 8/17/2025.
//

#pragma once
//
// Created by Matt on 8/15/2025.
//

#pragma once

#include <functional>
#include "juce_audio_basics/juce_audio_basics.h"

namespace meta
{
    template <typename FloatType>
    class BufferBuffer
    {
    public:
        BufferBuffer(int nChans, int bufferSize)
            : m_Buffer(nChans, bufferSize)
            , m_FIFO(bufferSize)
        {};

        void push(const juce::AudioBuffer<FloatType>& x)
        {
            jassert(x.getNumChannels() == m_Buffer.getNumChannels());
            const auto scope = m_FIFO.write(x.getNumSamples());

            for (int c = x.getNumChannels(); --c >= 0;)
            {
                if (scope.blockSize1 > 0)
                {
                    m_Buffer.copyFrom(c, scope.startIndex1, x, c, 0, scope.blockSize1);
                }

                if (scope.blockSize2 > 0)
                {
                    m_Buffer.copyFrom(c, scope.startIndex2, x, c, scope.blockSize1, scope.blockSize2);
                }
            }
        }

        void pop(juce::AudioBuffer<FloatType>& x)
        {
            jassert(x.getNumChannels() == m_Buffer.getNumChannels());
            x.clear();

            const auto scope = m_FIFO.read(x.getNumSamples());

            for (int c = x.getNumChannels(); --c >= 0;)
            {
                if (scope.blockSize1 > 0)
                {
                    x.copyFrom(c, 0, m_Buffer, c, scope.startIndex1, scope.blockSize1);
                }

                if (scope.blockSize2 > 0)
                {
                    x.copyFrom(c, scope.blockSize1, m_Buffer, c, scope.startIndex2, scope.blockSize2);
                }
            }
        }

        int getFreeSpace() { return m_FIFO.getFreeSpace(); }

    private:
        juce::AbstractFifo m_FIFO;
        juce::AudioBuffer<FloatType> m_Buffer;
    };

}