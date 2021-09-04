//
// Created by Matt on 8/9/2021.
//

#include <file_viewer/SpectrogramComponent.h>
#include <utility>

//static const int num_threads = 2;
static const int max_frames_per_thread = (44100 * 2) / 1024; // Guess at this (1 second of 48k, 2chan with a 2^10 frame) -- it can be futz'd w/.

MagPhaseChunkCalculator::MagPhaseChunkCalculator(
        const juce::dsp::AudioBlock<float> data,
        juce::dsp::AudioBlock<float> magnitude_out,
        juce::dsp::AudioBlock<float> phase_out,
        int fft_order, int x_overlap)
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
            auto data = r_Data.getChannelPointer(c) + frame_start;
            m_Calculator.calculate_window_unchecked(data, n_samps, mag, phase);
        }
    }
    sendChangeMessage();
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
    recalculateFrames();
}

void SpectrogramComponent::recalculateFrames()
{
    // just for shorthands
    const int n_chans = r_Data.getNumChannels();
    const float in_samps = r_Data.getNumSamples();

    const auto total_in_frames = std::ceil(in_samps / (float) m_FFTSize);
    const auto n_chunks = std::ceil(total_in_frames / max_frames_per_thread);
    const int in_chunk_size_samps = int(total_in_frames / n_chunks) * m_FFTSize;
    const int out_chunk_size_samps = in_chunk_size_samps * m_XOverlap;
    const int out_samps = out_chunk_size_samps * n_chunks;

    m_MagData.setSize(n_chans, out_samps);
    m_PhaseData.setSize(n_chans, out_samps);
    const auto in_data = r_Data.getArrayOfWritePointers();
    const auto mag_data = m_MagData.getArrayOfWritePointers();
    const auto phase_data = m_PhaseData.getArrayOfWritePointers();

    for (int chunk_i = 0; chunk_i < n_chunks; chunk_i++)
    {
        const auto in_chunk_start = chunk_i * in_chunk_size_samps;
        const auto out_chunk_start = chunk_i * out_chunk_size_samps;

        // we need to worry about the end of the stream on input, but not output.
        juce::dsp::AudioBlock<float> in_block(in_data, n_chans, in_chunk_start, std::min<int>(in_chunk_size_samps, in_samps - in_chunk_start));
        juce::dsp::AudioBlock<float> mag_block(mag_data, n_chans, out_chunk_start, out_chunk_size_samps);
        juce::dsp::AudioBlock<float> phase_block(phase_data, n_chans, out_chunk_start, out_chunk_size_samps);

        m_Components.emplace_back(new SpectrogramChunkComponent(juce::dsp::AudioBlock<float>(
                m_MagData, out_chunk_start),m_Gradient, m_FFTSize, m_XOverlap));
        addAndMakeVisible(m_Components.back().get());

        m_Calculations.emplace_back(new MagPhaseChunkCalculator(in_block, mag_block, phase_block, 10, 0));
        m_Calculations.back()->addChangeListener(m_Components.at(chunk_i).get());
        m_Calculations.back()->startThread();
    }
}

void SpectrogramComponent::resized()
{
    if (!m_Components.size()) { return; }
    auto local_bounds = getLocalBounds();

    const auto comp_width = local_bounds.getWidth() / m_Components.size();
    for (auto& component : m_Components)
    {
        component->setBounds(local_bounds.removeFromLeft(comp_width));
    }
}

SpectrogramComponent::~SpectrogramComponent()
{
    for (auto& thread : m_Calculations) { thread->stopThread(0); }
}

void SpectrogramComponent::paint(juce::Graphics& g)
{
    Component::paint(g);
}


SpectrogramChunkComponent::SpectrogramChunkComponent(
        const juce::dsp::AudioBlock<float>& data,
        const juce::ColourGradient& grad, int fft_size, int xOverlap)
    : r_MagData(data)
    , r_Gradient(grad)
    , m_FFTSize(fft_size)
    , m_XOverlap(xOverlap)
{}

void SpectrogramChunkComponent::recalculateSpectrogramImage()
{
    const auto x_size = int(std::ceil(r_MagData.getNumSamples() / m_FFTSize));
    const auto n_bins = m_FFTSize / 2;  // We only want the positive frequencies
    p_SpectrogramImage = std::make_unique<juce::Image>(juce::Image::RGB, x_size , n_bins, true);

    for (int s = x_size; --s >=0;)
    {
        for (int bin = n_bins; --bin > 0;)
        {
            // TODO: This scaling kinda works? it also cuts off the top. we'll see about tweaking this.
//            const int fft_i = (1.0f - std::exp (std::log ((float) bin / (float) n_bins) * 0.2f)) * n_bins;
            const auto fft_i = bin + n_bins;
            const int in_sample = s * m_FFTSize + fft_i;
            const auto sample_value = r_MagData.getChannelPointer(0)[in_sample];

            auto colour = r_Gradient.getColourAtPosition(sample_value);
            p_SpectrogramImage->setPixelAt(s, bin, colour);
        }
    }
}

void SpectrogramChunkComponent::dataRefreshed()
{
    recalculateSpectrogramImage();
    repaint();
}

void SpectrogramChunkComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::red);
    // it is possible to not have the data here ready. Wait until it is.
    if (p_SpectrogramImage == nullptr) { return; }
    g.drawImage(*p_SpectrogramImage, getLocalBounds().toFloat());
}

void SpectrogramChunkComponent::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    dataRefreshed();
}
