//
// Created by Matt on 8/9/2021.
//

#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_basics/juce_audio_basics.h>
#include <meta/dsp/MagPhaseCalculator.h>
#include <meta/util/container_helpers/TwoDimensionalHeap.h>
#include "SpectrogramSettings.h"


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


class SpectrogramChunkCalculator
    : public juce::ChangeListener
    , public juce::ChangeBroadcaster
{
public:
    SpectrogramChunkCalculator(const juce::dsp::AudioBlock<float>& data, juce::Image img, const SpectrogramSettings& settings);

    void changeListenerCallback(juce::ChangeBroadcaster* source) override;
    void recalculateSpectrogramImage();

private:
    juce::dsp::AudioBlock<float> r_MagData;
    juce::Image m_Img;
    const SpectrogramSettings& r_Settings;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SpectrogramChunkCalculator);
};


class SpectrogramComponent
    : public juce::Component
    , public SpectrogramSettings::Listener
    , juce::ChangeListener
{
public:
    SpectrogramComponent(juce::AudioBuffer<float>& data, SpectrogramSettings& settings);
    ~SpectrogramComponent();

    void paint(juce::Graphics& g) override;

    int getNFramesPerChan() const { return std::ceil(r_Data.getNumSamples() / r_Settings.getFFTSize()); }
    int getNChunksPerChan() const { return getNFramesPerChan(); }

    void changeListenerCallback(juce::ChangeBroadcaster* source) override;
    void gradientChanged(const SpectrogramSettings* settings) override;
    void fftChanged(const SpectrogramSettings* settings) override;

private:
    void recalculateFrames();

    juce::AudioBuffer<float>& r_Data;
    juce::AudioBuffer<float> m_MagData, m_PhaseData;
    SpectrogramSettings& r_Settings;

    std::unique_ptr<juce::Image> p_SpectrogramImage;
    std::vector<std::unique_ptr<SpectrogramChunkCalculator>> m_Chunks;
    std::vector<std::unique_ptr<MagPhaseChunkCalculator>> m_Calculations;
};


