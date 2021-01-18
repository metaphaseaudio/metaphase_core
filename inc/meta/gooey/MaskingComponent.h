//
// Created by Matt on 7/20/2020.
//

#pragma once

#include <juce_gui_basics/juce_gui_basics.h>


namespace meta
{
    class MaskingComponent
        : public juce::Component
    {
    public:
        explicit MaskingComponent(juce::Colour colour)
            : bg_colour(colour)
        {}

        void paint(juce::Graphics& r)
        {
            auto bounds = getLocalBounds();
            r.setColour(bg_colour);
            r.fillRect(bounds);
        }

        juce::Colour bg_colour;
    };
}