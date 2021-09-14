//
// Created by Matt on 9/9/2021.
//

#include <meta/gooey/MetaLookAndFeel.h>


void meta::MetaLookAndFeel::drawGradientColourPoint(juce::Graphics& g, const meta::GradientDesigner::ColourPoint& colourPoint)
{
    const auto pointerBounds = colourPoint.getLocalBounds().toFloat();
    const auto topMiddle = juce::Point<float>(pointerBounds.getCentreX(), pointerBounds.getTopRight().y);
    juce::Path pointer;
    pointer.addTriangle(pointerBounds.getBottomLeft(), pointerBounds.getBottomRight(), topMiddle);
    g.setColour(colourPoint.getPointColour());
    g.fillPath(pointer);
}