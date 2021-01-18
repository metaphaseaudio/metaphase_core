//
// Created by mzapp on 11/24/18.
//
#pragma once

#include <juce_gui_basics/juce_gui_basics.h>


namespace meta
{
    template <uint8_t R, uint8_t G, uint8_t B, uint8_t A>
    class ColourType
    {
    public:
        juce::Colour operator()() const { return juce::Colour(R,G,B,A); }
    };
}
