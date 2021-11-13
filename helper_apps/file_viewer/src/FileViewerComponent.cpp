#include <file_viewer/FileViewerComponent.h>
#include <utility>

//==============================================================================
FileViewerComponent::FileViewerComponent(juce::File filepath, juce::AudioBuffer<float>& data, double sample_rate, SpectrogramSettings& settings)
    : m_Filepath(std::move(filepath))
    , m_Data(data)
    , m_SampleRate(sample_rate)
    , m_Waveform(1)
    , m_Spectrogram(m_Data, settings)
    , m_SpecWavSlider(juce::Slider::SliderStyle::LinearHorizontal, juce::Slider::NoTextBox)
    , m_TimeRuler(juce::Range<float>(1, 100))
    , m_FreqRuler(juce::Range<float>(1, 100))
{
    m_Waveform.setClip(data, sample_rate);
    addAndMakeVisible(m_Spectrogram);
    addAndMakeVisible(m_Waveform);
    addAndMakeVisible(m_SpecWavSlider);
    addAndMakeVisible(m_TimeRuler);
    addAndMakeVisible(m_FreqRuler);
    m_SpecWavSlider.addListener(this);
    m_SpecWavSlider.setValue(5);
    setSize (600, 400);
}

//==============================================================================
void FileViewerComponent::paint (juce::Graphics& g)
{
    g.setColour(juce::Colours::black);
    g.fillAll();
}

void FileViewerComponent::resized()
{
    auto local_bounds = getLocalBounds();
    auto low_ctrls = local_bounds.removeFromBottom(22);
    auto time_ruler = local_bounds.removeFromBottom(22).removeFromLeft(local_bounds.getWidth() - 22);
    auto freq_ruler = local_bounds.removeFromRight(22);

    m_Waveform.setBounds(local_bounds);
    m_Spectrogram.setBounds(local_bounds);
    m_TimeRuler.setBounds(time_ruler);
    m_FreqRuler.setBounds(freq_ruler);
    m_SpecWavSlider.setBounds(low_ctrls.removeFromLeft(std::min(getWidth() / 10, 200)));
}

juce::File FileViewerComponent::getFilepath() const { return m_Filepath; }


void FileViewerComponent::sliderValueChanged(juce::Slider* slider)
{
    // slider's default is 0 - 10 float, div 10 to get alpha
    const auto alpha = slider->getValue() / 5.0;
    const auto inv_alpha = 2.0 - alpha;
    m_Waveform.setAlpha(alpha);
    m_Spectrogram.setAlpha(inv_alpha);
}

