//
// Created by Matt on 8/9/2021.
//

#include <file_viewer/SpectrogramComponent.h>
#include <utility>

static const int num_threads = 2;

MagPhaseChunkCalculator::MagPhaseChunkCalculator(
        const juce::dsp::AudioBlock<float>& data, juce::dsp::AudioBlock<float>& magnitude_out,
        juce::dsp::AudioBlock<float>& phase_out, int fft_order, int x_overlap)
    : juce::Thread("SpectrogramChunkCalc")
    , m_Calculator(fft_order)
    , r_Data(data)
    , r_Mag(magnitude_out)
    , r_Phase(phase_out)
    , m_FFTSize(m_Calculator.fft_size)
    , m_XOverlap(x_overlap)
{}

void MagPhaseChunkCalculator::run()
{
    const auto frames_to_calculate = std::ceil(r_Data.getNumSamples() / m_Calculator.fft_size);
    for (int c = 0; c < r_Data.getNumChannels(); c++)
    {
        for (int f = 0; f < frames_to_calculate; f++)
        {
            if (threadShouldExit()){ return; }
            const auto frame_start = f * m_FFTSize;
            const auto n_samps = std::min<int>(r_Data.getNumSamples() - frame_start, m_FFTSize);
            auto mag = r_Mag.getChannelPointer(c) + frame_start;
            auto phase = r_Phase.getChannelPointer(c) + frame_start;
            m_Calculator.calculate_window_unchecked(r_Data.getChannelPointer(c) + (f * m_FFTSize), n_samps, mag, phase);
        }
    }
}


SpectrogramComponent::SpectrogramComponent(juce::AudioBuffer<float>& data, int fftOrder, int xOverlap)
    : r_Data(data)
    , m_FFTOrder(fftOrder)
    , m_FFTSize(std::pow(2, fftOrder))
    , m_XOverlap(xOverlap)
    , m_NFrames(std::ceil((r_Data.getNumSamples() * r_Data.getNumChannels() * m_XOverlap) / m_FFTSize))
{
    m_Gradient.clearColours();
    m_Gradient.addColour(0, juce::Colours::black);
    m_Gradient.addColour(0.1428, juce::Colours::darkblue);
    m_Gradient.addColour(0.2857, juce::Colours::purple);
    m_Gradient.addColour(0.4285, juce::Colours::darkred);
    m_Gradient.addColour(0.5714, juce::Colours::red);
    m_Gradient.addColour(0.7142, juce::Colours::orange);
    m_Gradient.addColour(0.8571, juce::Colours::yellow);
    m_Gradient.addColour(1, juce::Colours::white);
    resetFrames();
    recalculateFrames();
}


void SpectrogramComponent::resetFrames()
{
    removeAllChildren();
    m_Components.clear();
    m_Calculations.clear();

    m_MagData.setSize(r_Data.getNumChannels(), m_NFrames * m_XOverlap * m_FFTSize);
    m_PhaseData.setSize(r_Data.getNumChannels(), m_NFrames * m_XOverlap * m_FFTSize);
    m_ColourData.realloc(r_Data.getNumChannels() * m_NFrames * m_FFTSize, sizeof(juce::Colour));

    m_MagData.clear(); m_PhaseData.clear();
    m_MagData.setSize(r_Data.getNumChannels(), r_Data.getNumSamples() * m_XOverlap);
    m_PhaseData.setSize(r_Data.getNumChannels(), r_Data.getNumSamples() * m_XOverlap);

    const int colourSampsPerChunk = m_NFrames * m_FFTSize;

    for (int i = num_threads; --i >= 0;)
    {
        m_Components.emplace_back(
            m_ColourData.twoD(),
            r_Data.getNumChannels(), colourSampsPerChunk, m_XOverlap);
        addAndMakeVisible(m_Components.back().get());
    }
}

void SpectrogramComponent::recalculateFrames()
{
    for (int t = 0; t < num_threads; t++)
    {
        juce::dsp::AudioBlock<float> in_block(r_Data, t * (r_Data.getNumSamples() / num_threads));
        juce::dsp::AudioBlock<float> mag_block(r_Data, t * (r_Data.getNumSamples() / num_threads));
        juce::dsp::AudioBlock<float> phase_block(r_Data, t * (r_Data.getNumSamples() / num_threads));
        m_Calculations.emplace_back(in_block, mag_block, phase_block);
    }
}

void SpectrogramComponent::resized()
{
    auto local_bounds = getLocalBounds();

    const auto chan_lane_height = local_bounds.getHeight() / r_Data.getNumChannels();

    const auto frames_per_chan = std::ceil(r_Data.getNumSamples() / m_FFTSize);

    for (int c = 0; c < r_Data.getNumChannels(); c++)
    {
        auto chan_bounds = local_bounds.removeFromTop(chan_lane_height).reduced(1);
        const auto frame_width = chan_bounds.getWidth() / frames_per_chan;
        for (int f = 0; f < frames_per_chan; f++)
        {
            auto i = c * frames_per_chan + f;
            m_Components.at(i)->setBounds(chan_bounds.removeFromLeft(frame_width));
        }
    }
}

SpectrogramComponent::~SpectrogramComponent()
{
    for (auto& thread : m_Calculations) { thread.stopThread(); }
}

void SpectrogramComponent::paint(juce::Graphics& g)
{
    Component::paint(g);
}


SpectrogramChunkComponent::SpectrogramChunkComponent(const juce::Colour** data, int nY, int nX, int xOverlap)
    : m_Data(data)
    , m_Y(nY)
    , m_X(nX)
    , m_XOverlap(xOverlap)
{}

void SpectrogramChunkComponent::dataRefreshed(){}

void SpectrogramChunkComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::red);
//    const auto max = float(m_FFTSize) / 2.0f; // 1/2 FFT size is the maximum value for both + and - frequencies
//    const auto bin_height = float(getHeight()) / (m_FFTSize / 2);
//    auto local_bounds = getLocalBounds().toFloat();
//    for (int i = 0; i < m_FFTSize / 2; i ++)
//    {
//        const auto bin_value = p_Frame->first.at(i) * 10;
//        const auto light_dark_ratio = bin_value / max;
//        const auto colour = m_Gradient.getColourAtPosition(light_dark_ratio);
//        g.setColour(colour);
//        const auto to_fill = local_bounds.removeFromBottom(bin_height);
//        g.fillRect(to_fill);
//        g.drawRect(to_fill);
//    }
}



//std::vector<std::vector<juce::Colour>>
//SpectrogramChunk::colourizeFrame(const std::vector<FFTFrame>& to_draw, const juce::ColourGradient& gradient, int fft_size)
//{
//    std::vector<SpectrogramFrame> rv;
//    const auto max = float(fft_size) / 2.0f; // 1/2 FFT size is the maximum value for both + and - frequencies
//    for (auto frame : to_draw)
//    {
//        SpectrogramFrame colour_frame;
//        for (auto bin_value : frame.first)
//        {
//            const auto light_dark_ratio = bin_value / max;
//            colour_frame.emplace_back(gradient.getColourAtPosition(light_dark_ratio));
//        }
//        rv.push_back(colour_frame)
//    }
//    return rv;
//}

