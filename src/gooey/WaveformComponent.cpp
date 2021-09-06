//
// Created by Matt on 4/19/2020.
//
#include <meta/gooey/WaveformComponent.h>


meta::WaveformComponent::WaveformComponent(size_t cacheSize, size_t sampsPerPixel)
    : m_ThumbCache(cacheSize)
    , m_Thumbnail(sampsPerPixel, m_DummyManager, m_ThumbCache)
{}

void meta::WaveformComponent::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds();
    g.setColour(getLookAndFeel().findColour(backgroundColourId));
    g.fillRect(bounds);
    const auto len = m_Thumbnail.getTotalLength();
    g.setColour(getLookAndFeel().findColour(foregroundColourId));
    m_Thumbnail.drawChannels(g, bounds, 0, len, 1.0);
}

void meta::WaveformComponent::setClip(const juce::AudioBuffer<float>& clip, double sampleRate)
{
    m_Thumbnail.reset(clip.getNumChannels(), sampleRate, clip.getNumSamples());
    m_Thumbnail.addBlock(0, clip, 0, clip.getNumSamples());
}
