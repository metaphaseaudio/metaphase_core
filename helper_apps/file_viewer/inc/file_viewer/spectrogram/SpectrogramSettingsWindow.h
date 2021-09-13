//
// Created by Matt on 9/6/2021.
//

#pragma once
#include <juce_gui_basics/juce_gui_basics.h>


class SpectrogramSettingsWindow
    : public juce::DocumentWindow
{
public:
    SpectrogramSettingsWindow();

private:
    void closeButtonPressed() override;
};


