#include <file_viewer/FileViewerComponent.h>
#include <utility>

//==============================================================================
FileViewerComponent::FileViewerComponent(juce::File filepath, juce::AudioBuffer<float>& data, double sample_rate)
    : m_Filepath(std::move(filepath))
    , m_Data(data)
    , m_SampleRate(sample_rate)
    , m_Waveform(1)
    , m_Spectrogram(m_Data, 9, 0)
{
    m_Waveform.setClip(data, sample_rate);
    addAndMakeVisible(m_Spectrogram);
    addAndMakeVisible(m_Waveform);
    setSize (600, 400);
}

//==============================================================================
void FileViewerComponent::paint (juce::Graphics& g)
{
    g.setFont (juce::Font (16.0f));
    g.setColour (juce::Colours::white);
    g.drawText (m_Filepath.getFileName(), getLocalBounds(), juce::Justification::centred, true);
}

void FileViewerComponent::resized()
{
    m_Spectrogram.setBounds(getLocalBounds());
//    m_Waveform.setBounds(getLocalBounds());
}

juce::File FileViewerComponent::getFilepath() const { return m_Filepath; }
