//
// Created by Matt on 8/1/2021.
//

#pragma once
#include <meta/testing/TestingComponentViewer.h>
#include <meta/util/file/AudioFileHelpers.h>
#include <juce_audio_utils/juce_audio_utils.h>
#include <meta/dsp/MagPhaseCalculator.h>

using FFTCalc = meta::dsp::MagPhaseCalculator<float>;
using FFTFrame = FFTCalc::MagPhaseFrame;

class SpectrogramComponent
    : public juce::Component
{
public:
    SpectrogramComponent(const std::vector<FFTFrame>& frames)
        : r_Frames(frames)
    {
        setSize(92, 1024 / 2.0f);
        m_Gradient.clearColours();
        m_Gradient.addColour(0, juce::Colours::black);
        m_Gradient.addColour(1, juce::Colours::white);
    }

    void render(int start_frame, int n_frames, int start_bin, int n_bins)
    {
        m_HorizWindowRange = juce::Range<int>(start_frame, start_frame + n_frames);
        m_VertWindowRange = juce::Range<int>(start_bin, start_bin + n_bins);

        for (auto frame : r_Frames)
        {
            for (auto bin_value : frame.first)
            {
                m_AmpDepth.setStart(std::min(bin_value, m_AmpDepth.getStart()));
                m_AmpDepth.setEnd(std::max(bin_value, m_AmpDepth.getEnd()));
            }
        }

        {
            juce::MessageManagerLock lock;
            repaint();
        }
    }

    void paint(juce::Graphics &g) override
    {
        g.fillAll(juce::Colours::red);
        for (int x = 0; x < r_Frames.size(); x++)
        {
            for (int y = 0; y < 1024; y++)
            {
                const auto pixel_value = r_Frames.at(x).first.at(y);
                const auto light_dark_ratio = pixel_value / m_AmpDepth.getEnd();
                g.setColour(m_Gradient.getColourAtPosition(light_dark_ratio));
                g.fillRect(x * 2, y, 2, 1);
            }
        }
    }


private:
    juce::Range<int> m_HorizWindowRange;
    juce::Range<int> m_VertWindowRange;
    juce::Range<float> m_AmpDepth;
    juce::ColourGradient m_Gradient;
    const std::vector<FFTFrame>& r_Frames;
};



TEST(GraphingPlot, basic_window)
{
    FFTCalc window_calc(10);
    auto file = juce::File::getSpecialLocation(juce::File::SpecialLocationType::userHomeDirectory)
                           .getChildFile("Downloads/car_interior_impulse_response.wav");
    std::unique_ptr<juce::AudioFormatReader> reader(meta::AudioFileHelpers::createReader(file));
    juce::AudioBuffer<float> in_data(reader->numChannels, reader->lengthInSamples);
    reader->read(in_data.getArrayOfWritePointers(), reader->numChannels, 0, reader->lengthInSamples);

    std::vector<FFTFrame> frames;

    auto spectrogram = std::make_unique<SpectrogramComponent>(frames);

    for (int i = 0; i < std::ceil(in_data.getNumSamples() / window_calc.fft_size); i++)
    {
        juce::AudioBuffer<float> tmp(1, window_calc.fft_size);
        tmp.clear();
        const auto start = i * window_calc.fft_size;
        const auto copy_len = std::min(window_calc.fft_size, in_data.getNumSamples() - start);
        tmp.copyFrom(0, 0, in_data, 0, start, copy_len);
        frames.push_back(window_calc.calculate_window(tmp, 0, 0));
    }

    const auto start_frame = 0;
    const auto n_frames = frames.size();
    spectrogram->render(start_frame, n_frames, 0, window_calc.fft_size / 2);
    auto viz = std::make_unique<meta::TestingComponentVisualizer>(spectrogram.release());
    viz->show();
}
