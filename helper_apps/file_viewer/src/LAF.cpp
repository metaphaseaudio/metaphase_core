//
// Created by Matt on 8/9/2021.
//

#include <file_viewer/LAF.h>
#include <meta/gooey/WaveformComponent.h>

LAF::LAF()
    : meta::MetaLookAndFeel()
{
    setColour(meta::WaveformComponent::ColourIds::backgroundColourId, juce::Colours::transparentBlack);
    setColour(meta::WaveformComponent::ColourIds::foregroundColourId, juce::Colours::greenyellow);
}



int LAF::getTabButtonBestWidth(juce::TabBarButton& button, int tabDepth)
{
    return LookAndFeel_V2::getTabButtonBestWidth(button, tabDepth);
}

