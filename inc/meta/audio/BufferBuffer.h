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
            : m_FIFO(bufferSize + 1)
            , m_Buffer(nChans, bufferSize + 1)
        {};

        void push(const juce::AudioBuffer<FloatType>& x)
        {
            jassert(x.getNumChannels() == m_Buffer.getNumChannels());
            jassert(m_FIFO.getFreeSpace() >= x.getNumSamples()); // overrun
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
            jassert(m_FIFO.getNumReady() >= x.getNumSamples()); // underrun
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

        void addAtOffsetFromReadHead(juce::AudioBuffer<FloatType>& x, int offset)
        {
            const auto totalRequiredSpace = x.getNumSamples() + offset;
            const auto extraRequiredSpace = totalRequiredSpace - m_FIFO.getNumReady();
            const auto totalToAdd = totalRequiredSpace - extraRequiredSpace;

            jassert(extraRequiredSpace <= m_FIFO.getFreeSpace());

            // Use prepare to read so as to not update the read head
            int startIndex1, blockSize1, startIndex2, blockSize2;
            m_FIFO.prepareToRead(totalToAdd, startIndex1, blockSize1, startIndex2, blockSize2);

            const auto block1WriteSize = std::max(blockSize1 - offset, 0);
            const auto block2WriteSize = totalToAdd - block1WriteSize;
            const auto block2Offset = std::max(offset - blockSize1, 0);

            for (auto c = x.getNumChannels(); --c >= 0;)
            {
                if (block1WriteSize > 0)
                {
                    m_Buffer.addFrom(c, startIndex1 + offset, x, c, 0, block1WriteSize);
                }

                if (block2WriteSize > 0)
                {
                    m_Buffer.addFrom(c, startIndex2 + block2Offset, x, c, blockSize1, block2WriteSize);
                }
            }

            // Adding off the end == pushing the remaining data
            if (extraRequiredSpace)
            {
                juce::AudioBuffer<FloatType> view;
                view.setDataToReferTo(x.getArrayOfWritePointers(), x.getNumChannels(), x.getNumSamples() - extraRequiredSpace, extraRequiredSpace);
                push(view);
            }
        }

        [[ nodiscard ]] int getFreeSpace() const { return m_FIFO.getFreeSpace(); }
        [[ nodiscard ]] int getNumSamplesReady() const { return m_FIFO.getNumReady(); }

    private:
        juce::AbstractFifo m_FIFO;
        juce::AudioBuffer<FloatType> m_Buffer;
    };

}