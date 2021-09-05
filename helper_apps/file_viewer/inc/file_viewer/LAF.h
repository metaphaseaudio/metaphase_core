//
// Created by Matt on 8/9/2021.
//

#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

class LAF
    : public juce::LookAndFeel_V4
{
public:
    LAF();

    int getTabButtonBestWidth(juce::TabBarButton& button, int tabDepth) override;
};


