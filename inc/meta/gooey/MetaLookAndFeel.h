//
// Created by Matt on 9/9/2021.
//

#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include <meta/gooey/GradientDesigner.h>

namespace meta
{
    class MetaLookAndFeel
            : public juce::LookAndFeel_V4
    {
    public:
        MetaLookAndFeel();
        void drawGradientDesignerColourPoint(juce::Graphics& g, const meta::GradientDesigner::ColourPoint& colourPoint);
        void drawGradientDesignerDisplay(juce::Graphics& g, const meta::GradientDesigner::Display& display);
    };
}
