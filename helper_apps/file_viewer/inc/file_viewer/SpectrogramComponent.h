//
// Created by Matt on 8/9/2021.
//

#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_basics/juce_audio_basics.h>
#include <meta/dsp/MagPhaseCalculator.h>
#include <meta/util/container_helpers/TwoDimensionalHeap.h>

class MagPhaseChunkCalculator
    : public juce::Thread
{
public:
    using FFTFrame = meta::dsp::MagPhaseCalculator<float>::MagPhaseFrame;

    MagPhaseChunkCalculator(
        const juce::dsp::AudioBlock<float>& data,
        juce::dsp::AudioBlock<float>& magnitude_out,
        juce::dsp::AudioBlock<float>& phase_out,
        int fft_order, int x_overlap
    );

    void run() override;

private:
    meta::dsp::MagPhaseCalculator<float> m_Calculator;
    const juce::dsp::AudioBlock<float>& r_Data;
    juce::dsp::AudioBlock<float>& r_Mag, r_Phase;
    int m_FFTSize, m_XOverlap;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MagPhaseChunkCalculator);
};


class SpectrogramChunkComponent
    : public juce::Component
{
public:
    SpectrogramChunkComponent(const juce::Colour** data, int nY, int nX, int xOverlap);
    void dataRefreshed();
    void paint(juce::Graphics& g) override;

private:
    int m_XOverlap, m_Y, m_X;
    const juce::Colour** m_Data;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SpectrogramChunkComponent);
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

    int m_FFTOrder, m_FFTSize, m_XOverlap, m_NFrames;
    juce::AudioBuffer<float>& r_Data;
    juce::AudioBuffer<float> m_MagData, m_PhaseData;
    meta::TwoDimensionalHeap<juce::Colour> m_ColourData;
    juce::ColourGradient m_Gradient;
    std::vector<std::unique_ptr<SpectrogramChunkComponent>> m_Components;
    std::vector<MagPhaseChunkCalculator> m_Calculations;
};


