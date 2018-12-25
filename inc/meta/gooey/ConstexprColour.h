//
// Created by mzapp on 11/24/18.
//
#pragma once

#include <JuceHeader.h>

namespace meta
{
    class ConstexprColour
    {
    public:
        constexpr ConstexprColour(uint8_t rin, uint8_t gin, uint8_t bin, uint8_t ain)
            : r(rin), g(gin), b(bin), a(ain)
        {};

        constexpr ConstexprColour(const juce::Colour& c)
            : r(c.getRed())
            , g(c.getGreen())
            , b(c.getBlue())
            , a(c.getAlpha())
        {}

        juce::Colour operator()() const { return juce::Colour(r,g,b,a); }

    private:
        uint8_t r,g,b,a;
    };
}
