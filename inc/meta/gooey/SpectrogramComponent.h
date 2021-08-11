//
// Created by Matt on 8/9/2021.
//

#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_basics/juce_audio_basics.h>
#include <inc/meta/dsp/MagPhaseCalculator.h>


namespace meta
{
    class SpectrogramComponent
        : public juce::Component
    {
        using FFTFrame = meta::dsp::MagPhaseCalculator<float>::MagPhaseFrame;
    public:
        SpectrogramComponent(const std::vector<FFTFrame>& frames)
            : r_Frames(frames)
        {
            m_Gradient.clearColours();
            m_Gradient.addColour(0, juce::Colours::black);
            m_Gradient.addColour(1, juce::Colours::white);
        }

        void draw(int start_frame, int n_frames, int start_bin, int n_bins)
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

            repaint();
        }

        void paint(juce::Graphics &g) override
        {
            g.fillAll(juce::Colours::red);
            for (int x = 0; x < r_Frames.size(); x++)
            {
                for (int y = 0; y < r_Frames.at(x).first.size(); y++)
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

}
