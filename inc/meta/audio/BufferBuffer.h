//
// Created by Matt on 8/17/2025.
//

#pragma once
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

        void peek(juce::AudioBuffer<FloatType>& x, int offset=0)
        {
            jassert(offset >= 0);
            jassert(x.getNumChannels() == m_Buffer.getNumChannels());
            jassert(x.getNumSamples() + offset < m_FIFO.getNumReady());

            int startIndex1, blockSize1, startIndex2, blockSize2;
            m_FIFO.prepareToRead(x.getNumSamples() + offset, startIndex1, blockSize1, startIndex2, blockSize2);

            const auto block1ReadSize = std::max(blockSize1 - offset, 0);
            const auto block2Offset = std::max(offset - blockSize1, 0);
            const auto block2ReadSize = x.getNumSamples() - block1ReadSize - block2Offset;

            for (int c = x.getNumChannels(); --c >= 0;)
            {
                if (block1ReadSize > 0)
                {
                    x.copyFrom(c, 0, m_Buffer, c, startIndex1 + offset, block1ReadSize);
                }

                if (block2ReadSize > 0)
                {
                    x.copyFrom(c, block1ReadSize, m_Buffer, c, startIndex2 + block2Offset, block2ReadSize);
                }
            }
        }

        void pushZeros(int nZeros)
        {
            auto scope = m_FIFO.write(nZeros);

            if (scope.blockSize1 > 0)
            {
                m_Buffer.clear(scope.startIndex1, scope.blockSize1);
            }

            if (scope.blockSize2 > 0)
            {
                m_Buffer.clear(scope.startIndex2, scope.blockSize2);
            }
        }

        void addAtOffsetFromReadHead(juce::AudioBuffer<FloatType>& x, int offset, float gain=1.0f)
        {
            jassert(offset >= 0);
            jassert(x.getNumChannels() == m_Buffer.getNumChannels());

            const auto totalRequiredSpace = x.getNumSamples() + offset;  // How much space is needed overall
            const auto leadingZeros = std::max(offset - m_FIFO.getNumReady(), 0);  // Special case for offset > ready
            const auto extraRequiredSpace = std::max(totalRequiredSpace - m_FIFO.getNumReady() - leadingZeros, 0);
            jassert(extraRequiredSpace <= m_FIFO.getFreeSpace());

            pushZeros(extraRequiredSpace + leadingZeros);

            // Use prepare to read so as to retrieve, but not update the read head
            int startIndex1, blockSize1, startIndex2, blockSize2;
            m_FIFO.prepareToRead(totalRequiredSpace, startIndex1, blockSize1, startIndex2, blockSize2);

            const auto block1WriteSize = std::max(blockSize1 - offset, 0);
            const auto block2Offset = std::max(offset - blockSize1, 0);
            const auto block2WriteSize = x.getNumSamples() - block1WriteSize - block2Offset;

            for (auto c = x.getNumChannels(); --c >= 0;)
            {
                if (block1WriteSize > 0)
                {
                    m_Buffer.addFrom(c, startIndex1 + offset, x, c, 0, block1WriteSize, gain);
                }

                if (block2WriteSize > 0)
                {
                    m_Buffer.addFrom(c, startIndex2 + block2Offset, x, c, block1WriteSize, block2WriteSize, gain);
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

        void discard(int nToDiscard)
        {
            m_FIFO.read(nToDiscard);
        }

        void transferFromOther(BufferBuffer<FloatType>& other, int nToTransfer)
        {
            auto scope = m_FIFO.write(nToTransfer);

            juce::AudioBuffer<FloatType> tmp;

            tmp.setDataToReferTo(
                m_Buffer.getArrayOfWritePointers(),
                m_Buffer.getNumChannels(),
                scope.blockSize1, scope.startIndex1
            );
            other.pop(tmp);

            tmp.setDataToReferTo(
                m_Buffer.getArrayOfWritePointers(),
                m_Buffer.getNumChannels(),
                scope.blockSize2, scope.startIndex2
            );
            other.pop(tmp);
        }

        [[ nodiscard ]] int getFreeSpace() const { return m_FIFO.getFreeSpace(); }
        [[ nodiscard ]] int getNumSamplesReady() const { return m_FIFO.getNumReady(); }

    private:
        juce::AbstractFifo m_FIFO;
        juce::AudioBuffer<FloatType> m_Buffer;
    };

}