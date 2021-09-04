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
    , public juce::ChangeBroadcaster
{
public:
    MagPhaseChunkCalculator(
        const juce::dsp::AudioBlock<float> data,
        juce::dsp::AudioBlock<float> magnitude_out,
        juce::dsp::AudioBlock<float> phase_out,
        int fft_order, int x_overlap
    );

    void run() override;

private:
    meta::dsp::MagPhaseCalculator<float> m_Calculator;
    const juce::dsp::AudioBlock<float> r_Data;
    juce::dsp::AudioBlock<float> r_Mag, r_Phase;
    int m_FFTSize, m_XOverlap;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MagPhaseChunkCalculator);
};


class SpectrogramChunkComponent
    : public juce::Component
    , public juce::ChangeListener
{
public:
    SpectrogramChunkComponent(const juce::dsp::AudioBlock<float>& data, const juce::ColourGradient& grad, int fftSize, int xOverlap);
    void dataRefreshed();
    void paint(juce::Graphics& g) override;

    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

private:
    void recalculateSpectrogramImage();

    juce::dsp::AudioBlock<float> r_MagData;
    const juce::ColourGradient& r_Gradient;
    std::unique_ptr<juce::Image> p_SpectrogramImage;
    int m_FFTSize, m_XOverlap;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SpectrogramChunkComponent);
};


class SpectrogramComponent
    : public juce::Component
{
public:
    SpectrogramComponent(juce::AudioBuffer<float>& data, int fftOrder, int xOverlap);
    ~SpectrogramComponent();
    void resized() override;
    void paint(juce::Graphics& g) override;

    int getNFramesPerChan() const { return std::ceil(r_Data.getNumSamples() / m_FFTSize); }
    int getNChunksPerChan() const { return getNFramesPerChan(); }

private:
    void resetFrames();
    void resetComponents();
    void recalculateFrames();

    juce::AudioBuffer<float>& r_Data;
    juce::AudioBuffer<float> m_MagData, m_PhaseData;
    int m_FFTOrder, m_FFTSize, m_XOverlap;

    juce::ColourGradient m_Gradient;

    std::vector<std::unique_ptr<SpectrogramChunkComponent>> m_Components;
    std::vector<std::unique_ptr<MagPhaseChunkCalculator>> m_Calculations;

};


