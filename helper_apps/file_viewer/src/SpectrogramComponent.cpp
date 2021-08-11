//
// Created by Matt on 8/9/2021.
//

#include <file_viewer/SpectrogramComponent.h>
#include <utility>
SpectrogramChunkCalculator::SpectrogramChunkCalculator(
        const juce::dsp::AudioBlock<float>& data,
        std::vector<std::vector<FFTFrame>>::iterator& output,
        int fft_order, int x_overlap
)
    : juce::Thread("SpectrogramChunkCalc")
    , m_Calculator(fft_order)
    , r_Data(data)
    , r_Output(std::move(output))
    , m_FFTSize(m_Calculator.fft_size)
    , m_XOverlap(x_overlap)
{}


void SpectrogramChunkCalculator::run()
{
    const auto frames_per_chan = std::ceil(r_Data.getNumSamples() / m_Calculator.fft_size);
    for (int c = 0; c < r_Data.getNumChannels(); c++)
    {
        for (int i = 0; i < frames_per_chan; i++)
        {
            if (threadShouldExit()){ return; }
            *(r_Output + c) = m_Calculator.calculate_window(r_Data, c, i * int(m_Calculator.fft_size));
        }
    }
}


SpectrogramComponent::SpectrogramComponent(juce::AudioBuffer<float>& data, int fftOrder, int xOverlap)
    : r_Data(data)
    , m_FFTOrder(fftOrder)
    , m_FFTSize(std::pow(2, fftOrder))
    , m_XOverlap(xOverlap)
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
    m_Frames.clear();
    const auto frames_per_chan = std::ceil(r_Data.getNumSamples() / m_FFTSize);
    m_Frames.resize(r_Data.getNumChannels() * frames_per_chan);

    for (auto frame : m_Frames)
    {
        m_Components.push_back(std::make_unique<FFTFrameComponent>(&frame, m_FFTSize, m_Gradient));
        addAndMakeVisible(m_Components.back().get());
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

void SpectrogramComponent::recalculateFrames()
{
    const auto max_num_threads = 2;
    m_Calculations.clear();
    for (int t = 0; t < max_num_threads; t++)
    {
        juce::dsp::AudioBlock<float> block(r_Data, t * (r_Data.getNumSamples() / max_num_threads));
        m_Calculations.emplace_back(block);
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

FFTFrameComponent::FFTFrameComponent(FFTFrameComponent::FFTFrame* data, int fft_size, juce::ColourGradient  gradient)
    : m_FFTSize(fft_size)
    , m_Gradient(std::move(gradient))
    , p_Frame(data)
{}


void FFTFrameComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::red);
    const auto max = float(m_FFTSize) / 2.0f; // 1/2 FFT size is the maximum value for both + and - frequencies
    const auto bin_height = float(getHeight()) / (m_FFTSize / 2);
    auto local_bounds = getLocalBounds().toFloat();
    for (int i = 0; i < m_FFTSize / 2; i ++)
    {
        const auto bin_value = p_Frame->first.at(i) * 10;
        const auto light_dark_ratio = bin_value / max;
        const auto colour = m_Gradient.getColourAtPosition(light_dark_ratio);
        g.setColour(colour);
        const auto to_fill = local_bounds.removeFromBottom(bin_height);
        g.fillRect(to_fill);
        g.drawRect(to_fill);
    }
}


std::vector<std::vector<juce::Colour>>
SpectrogramChunk::colourizeFrame(const std::vector<FFTFrame>& to_draw, const juce::ColourGradient& gradient, int fft_size)
{
    std::vector<SpectrogramFrame> rv;
    const auto max = float(fft_size) / 2.0f; // 1/2 FFT size is the maximum value for both + and - frequencies
    for (auto frame : to_draw)
    {
        SpectrogramFrame colour_frame;
        for (auto bin_value : frame.first)
        {
            const auto light_dark_ratio = bin_value / max;
            colour_frame.emplace_back(gradient.getColourAtPosition(light_dark_ratio));
        }
        rv.push_back(colour_frame)
    }
    return rv;
}

