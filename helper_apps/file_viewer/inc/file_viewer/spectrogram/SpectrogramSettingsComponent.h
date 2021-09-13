//
// Created by Matt on 9/6/2021.
//

#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include <inc/meta/gooey/GradientDesigner.h>


class SpectrogramSettingsComponent
    : public juce::Component
{
public:
    SpectrogramSettingsComponent();
    void resized() override;

private:
    meta::GradientDesigner m_Gradient;
};


