//
// Created by Matt on 2/24/2026.
//

#pragma once
#include <vector>
#include <juce_audio_basics/juce_audio_basics.h>


namespace meta
{

template <typename T>
struct DefaultAllocator
{
    T* allocate(std::size_t count)
    {
        return static_cast<T*>(::operator new(sizeof(T) * count));
    }

    void deallocate(T* ptr, std::size_t /*count*/)
    {
        ::operator delete(ptr);
    }
};

template <typename FloatType, typename Allocator=DefaultAllocator<FloatType>>
class AudioBuffer
{
public:
    AudioBuffer(int channels, int samples, Allocator allocator = Allocator{})
        : m_NumChannels(channels)
        , m_NumSamples(samples)
        , m_Allocator(allocator)
        , m_ChannelPtrs(channels, nullptr)
    {
        const std::size_t totalSamples = static_cast<std::size_t>(m_NumChannels) * m_NumSamples;

        p_Data = m_Allocator.allocate(totalSamples);

        for (int ch = 0; ch < m_NumChannels; ++ch)
        {
            m_ChannelPtrs[ch] = p_Data + (ch * m_NumSamples);
        }

        // Attach external memory to AudioBuffer
        m_Buffer.setDataToReferTo(m_ChannelPtrs.data(), m_NumChannels, m_NumSamples);
    }

    juce::AudioBuffer<float>& getJUCEBuffer() { return m_Buffer; }

    void clear() { m_Buffer.clear(); }

    ~AudioBuffer()
    {
        m_Allocator.deallocate(p_Data, static_cast<std::size_t>(m_NumChannels) * m_NumSamples);
    }

private:
    int m_NumChannels{};
    int m_NumSamples{};

    Allocator m_Allocator;

    float* p_Data = nullptr;
    std::vector<float*> m_ChannelPtrs;

    juce::AudioBuffer<float> m_Buffer;
};
} // meta