//
// Created by Matt on 8/9/2021.
//

#include <file_viewer/TabCloseButton.h>

void TabCloseButton::paintButton(juce::Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    auto& laf = getLookAndFeel();
    const juce::Path cross = laf.getCrossShape(getLocalBounds().getHeight());
    const juce::Colour base_colour = juce::Colours::grey;
    const juce::Colour over_colour = juce::Colours::white;
    const juce::Colour click_colour = juce::Colours::lightgrey;
    g.setColour(shouldDrawButtonAsDown ? click_colour : shouldDrawButtonAsHighlighted ? over_colour : base_colour);
    g.fillPath(cross, cross.getTransformToScaleToFit(getLocalBounds().reduced (2).toFloat(), true));
}
