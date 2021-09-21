//
// Created by Matt on 8/8/2021.
//

#pragma once

namespace CommandIDs
{
    static const int open_file            = 0x30000;
    static const int close_file           = 0x30001;
    static const int audio_settings       = 0x30200;
    static const int spectrogram_settings = 0x30300;
    static const int about                = 0x30400;
}

namespace AppState
{
    static const std::string OPEN_FILES = "open_files";
    static const std::string RECENT_FILES = "recent_files";
    static const std::string MAIN_WINDOW_POSITION = "main_window_position";
}

namespace Defaults
{
    static const juce::ColourGradient gradient() {
        std::vector<juce::Colour> colours = {
                juce::Colours::black,
                juce::Colours::darkblue,
                juce::Colours::purple,
                juce::Colours::darkred,
                juce::Colours::red,
                juce::Colours::orange,
                juce::Colours::yellow,
                juce::Colours::white,
        };

        juce::ColourGradient grad;
        const auto increment = 1.0f / (colours.size() - 1);
        float position = 0;
        for (auto& colour : colours)
        {
            const auto log_position = meta::Interpolate<float>::parabolic(0.0f, 1.0f, position, 7);
            grad.addColour(log_position, colour);
            position += increment;
        }
        return grad;
    };
}