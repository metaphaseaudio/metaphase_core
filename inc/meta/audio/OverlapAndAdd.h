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

        OverlapAndAdd(int nChans, int nSamps, int nOverlap, int maxExpectedSamples=0)
            : m_NSamps(nSamps)
            , m_Stride(static_cast<int>(std::round(static_cast<float>(nSamps) / static_cast<float>(nOverlap))))
            , m_WindowGain(1.0f / nOverlap)
            , m_ChunkTmp(nChans, nSamps)
            , m_InputBuffer(nChans, nSamps + maxExpectedSamples)
            , m_OutputBuffer(nChans, nSamps + maxExpectedSamples)
            , m_CurrentOffset(m_NSamps)
        {
            m_ChunkTmp.clear();

            m_OutputBuffer.pushZeros(m_NSamps);
            m_CurrentOffset = m_NSamps;
        };

        std::function<void(juce::AudioBuffer<float>& chunk)> processChunk;

        [[ nodiscard ]] int getLatency() const { return m_NSamps; }

        void process(juce::AudioBuffer<FloatType>& x)
        {
            m_InputBuffer.push(x);

            while (m_InputBuffer.getNumSamplesReady() > m_NSamps)
            {
                // Process and discard the input
                m_InputBuffer.peek(m_ChunkTmp);
                processChunk(m_ChunkTmp);
                m_OutputBuffer.addAtOffsetFromReadHead(m_ChunkTmp, m_CurrentOffset, m_WindowGain);

                m_InputBuffer.discard(m_Stride);
                m_CurrentOffset += m_Stride;
            }

            m_OutputBuffer.pop(x);
            m_CurrentOffset -= x.getNumSamples();
        };

    private:
        const int m_NSamps, m_Stride;
        float m_WindowGain;
        int m_CurrentOffset;
        juce::AudioBuffer<FloatType> m_ChunkTmp;
        BufferBuffer<FloatType> m_InputBuffer, m_OutputBuffer;
    };
}