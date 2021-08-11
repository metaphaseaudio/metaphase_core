//
// Created by Matt on 8/9/2021.
//

#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_basics/juce_audio_basics.h>
#include <meta/dsp/MagPhaseCalculator.h>

class SpectrogramChunkCalculator
    : public juce::Thread
{
public:
    using FFTFrame = meta::dsp::MagPhaseCalculator<float>::MagPhaseFrame;

    SpectrogramChunkCalculator(
        const juce::dsp::AudioBlock<float>& data,
        std::vector<std::vector<FFTFrame>>::iterator& output,
        int fft_order, int x_overlap
    );

    void run() override;

private:
    meta::dsp::MagPhaseCalculator<float> m_Calculator;
    juce::dsp::AudioBlock<float> r_Data;
    std::vector<std::vector<FFTFrame>>::iterator r_Output;
    int m_FFTSize, m_XOverlap;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SpectrogramChunkCalculator);
};


class FFTFrameComponent
    : public juce::Component
{
    using FFTFrame = meta::dsp::MagPhaseCalculator<float>::MagPhaseFrame;
public:
    FFTFrameComponent(FFTFrame* data, int fft_size, juce::ColourGradient gradient);
    void setData(FFTFrame* data) { p_Frame = data; };
    void paint(juce::Graphics& g) override;

private:
    const int m_FFTSize;
    const juce::ColourGradient m_Gradient;
    FFTFrame* p_Frame;
};


class SpectrogramChunk
    : public juce::Component
{
    using SpectrogramFrame = std::vector<juce::Colour>;
    using FFTFrame = meta::dsp::MagPhaseCalculator<float>::MagPhaseFrame;
public:

private:
    std::vector<SpectrogramFrame> m_Frames;
};

class SpectrogramComponent
    : public juce::Component
{
    using FFTFrame = meta::dsp::MagPhaseCalculator<float>::MagPhaseFrame;
public:
    SpectrogramComponent(juce::AudioBuffer<float>& data, int fftOrder, int xOverlap);
    ~SpectrogramComponent();
    void resized() override;

    void paint(juce::Graphics& g) override;

private:
    void resetFrames();
    void recalculateFrames();

    int m_FFTOrder, m_FFTSize, m_XOverlap;
    juce::AudioBuffer<float>& r_Data;
    juce::ColourGradient m_Gradient;
    std::vector<FFTFrame> m_Frames;
    std::vector<std::unique_ptr<FFTFrameComponent>> m_Components;
    std::vector<SpectrogramChunkCalculator> m_Calculations;
};


