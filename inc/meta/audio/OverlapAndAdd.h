//
// Created by Matt on 8/20/2025.
//

#pragma once

#include "juce_audio_basics/juce_audio_basics.h"

namespace meta
{
    template <typename FloatType>
    class OverlapAndAdd
    {
    public:
        OverlapAndAdd(int nChans, int nSamps, int nOverlap)
            : m_NSamps(nSamps)
            , m_NOverlap(nOverlap)
            , m_Stride(static_cast<int>(std::round(static_cast<float>(nSamps) / static_cast<float>(nOverlap))))
            , m_Tail((nOverlap - 1) * m_Stride)
            , m_Out(nChans, nSamps + m_Tail)
            , m_In(nChans, nSamps + m_Tail)
            , m_ChunkTmp(nChans, nSamps)
        {
            m_Out.clear();
            m_In.clear();
            m_ChunkTmp.clear();
        };

        std::function<void(const juce::AudioBuffer<float>& input, juce::AudioBuffer<float>& output)> forEachChunk;

        [[ nodiscard ]] int getLatency() const { return m_NSamps / 2; }

        void process(juce::AudioBuffer<FloatType>& x)
        {
            m_InputBuffer.push(x);

            if (m_InputBuffer.getUsedSpace())

            jassert(x.getNumChannels() == m_Out.getNumChannels());
            // jassert(x.getNumSamples() == m_NSamps);

            for (int c = x.getNumChannels(); --c >= 0;)
            {
                // Copy the data into the input buffer at the end
                m_In.copyFrom(c, m_Tail, x, c, 0, m_NSamps);
                // Copy the output data into the buffer from the front
                x.copyFrom(c, 0, m_Out, c, 0, m_NSamps);
            }

            auto in = m_In.getArrayOfWritePointers();
            auto out = m_Out.getArrayOfWritePointers();

            // Process the chunks
            for (int i = 0; i < m_NOverlap; i++)
            {
                juce::AudioBuffer<float> chunk;

                // Create a view of the input for each chunk
                const auto offset = m_Stride * i;
                chunk.setDataToReferTo(in, m_In.getNumChannels(), offset, m_NSamps);

                // process the chunk
                forEachChunk(chunk, m_ChunkTmp);

                // Add the chunk to the output buffer
                for (int c = x.getNumChannels(); --c >= 0;)
                {
                    m_Out.addFrom(c, offset, m_ChunkTmp, c, 0, m_NSamps, 1.0f / m_NOverlap);
                }
            }

            const auto nTailBytes = sizeof(FloatType) * m_Tail;
            const auto nChunkBytes = sizeof(FloatType) * m_NSamps;

            for (int c = x.getNumChannels(); --c >= 0;)
            {
                x.copyFrom(c, 0, m_Out, c, 0, m_NSamps);  // Copy the current value out to the caller
                std::memcpy(out[c], out[c] + m_NSamps, nTailBytes);  // Move the tail to the start
                std::memcpy(in[c], in[c] + m_NSamps, nTailBytes);  // do the same with the input buffer

                std::memset(out[c] + m_Tail, 0, nChunkBytes);  // Clear the remaining room for adding.
                // No need to do that with the input, that gets copied, not added.
            }
        };

    private:
        const int m_NSamps, m_NOverlap, m_Stride, m_Tail;
        juce::AudioBuffer<FloatType> m_Out, m_In, m_ChunkTmp;
        BufferBuffer<FloatType> m_InputBuffer, m_OutputBuffer;
    };
}