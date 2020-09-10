//
// Created by Matt on 4/19/2020.
//
#pragma once

#include <JuceHeader.h>

namespace meta
{
    class WaveformComponent
        : public juce::Component
    {
    public:
        explicit WaveformComponent(size_t sampsPerPixel, size_t cacheSize=10);
        void paint (juce::Graphics&) override;
        void setClip(const juce::AudioBuffer<float>& clip, double sampleRate);
        void clear() { m_Thumbnail.clear(); };

    private:
        juce::AudioFormatManager m_DummyManager;
        juce::AudioThumbnailCache m_ThumbCache;
        juce::AudioThumbnail m_Thumbnail;
    };
}
