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
        WaveformComponent(juce::AudioFormatManager& mgr, size_t cacheSize, size_t sampsPerPixel);
        void paint (juce::Graphics&) override;
        void setClip(const juce::AudioBuffer<float>& clip, double sampleRate);

    private:
        juce::AudioThumbnailCache m_ThumbCache;
        juce::AudioThumbnail m_Thumbnail;
    };
}
