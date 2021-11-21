//
// Created by Matt on 9/9/2021.
//

#include <meta/gooey/MetaLookAndFeel.h>
#include <inc/meta/gooey/WaveformComponent.h>

meta::MetaLookAndFeel::MetaLookAndFeel()
{
    setColour(WaveformComponent::ColourIds::backgroundColourId, juce::Colours::black);
    setColour(WaveformComponent::ColourIds::foregroundColourId, juce::Colours::yellowgreen);
}

void meta::MetaLookAndFeel::drawGradientDesignerColourPoint(juce::Graphics& g, const meta::GradientDesigner::ColourPoint& colourPoint)
{
    const auto pointerBounds = colourPoint.getLocalBounds().reduced(1).toFloat();
    const auto topMiddle = juce::Point<float>(pointerBounds.getCentreX(), pointerBounds.getTopRight().y);
    juce::Path pointer;
    pointer.addTriangle(pointerBounds.getBottomLeft(), pointerBounds.getBottomRight(), topMiddle);
    g.setColour(colourPoint.getPointColour());
    g.fillPath(pointer);
    g.setColour(juce::Colours::darkgrey);
    g.strokePath(pointer, juce::PathStrokeType(1.0f));
}

void meta::MetaLookAndFeel::drawGradientDesignerDisplay(juce::Graphics& g, const meta::GradientDesigner::Display& display)
{
    g.setGradientFill(display.gradient);
    g.fillRoundedRectangle(display.getLocalBounds().reduced(1).toFloat(), display.getHeight() / 2.0f);
    g.setColour(juce::Colours::darkgrey.darker());
    g.drawRoundedRectangle(display.getLocalBounds().reduced(1).toFloat(), display.getHeight() / 3.0f, 1.0f);
}
