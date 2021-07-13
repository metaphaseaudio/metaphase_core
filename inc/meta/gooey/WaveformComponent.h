//
// Created by Matt on 4/19/2020.
//
#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_formats/juce_audio_formats.h>
#include <juce_audio_utils/juce_audio_utils.h>


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
