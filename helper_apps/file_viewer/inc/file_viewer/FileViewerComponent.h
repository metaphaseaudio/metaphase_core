#pragma once
#include <juce_gui_extra/juce_gui_extra.h>
#include <juce_audio_basics/juce_audio_basics.h>
#include <meta/gooey/WaveformComponent.h>
#include <file_viewer/SpectrogramComponent.h>

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
#pragma once

class FileViewerComponent
    : public juce::Component
    , juce::Slider::Listener
{
    using FFTFrame = meta::dsp::MagPhaseCalculator<float>::MagPhaseFrame;
public:
    FileViewerComponent(juce::File filepath, juce::AudioBuffer<float>& data, double sample_rate);
    void paint (juce::Graphics&) override;
    void resized() override;
    juce::File getFilepath() const;

private:
    void sliderValueChanged(juce::Slider* slider);
    juce::File m_Filepath;
    juce::AudioBuffer<float> m_Data;
    double m_SampleRate;
    meta::WaveformComponent m_Waveform;
    SpectrogramComponent m_Spectrogram;
    juce::Slider m_SpecWavSlider;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FileViewerComponent)
};
