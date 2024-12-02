//
// Created by Matt on 4/28/2023.
//

#pragma once

#include <juce_graphics/juce_graphics.h>


namespace meta
{
    class PerspectiveImageEffectFilter
        : public juce::ImageEffectFilter
    {
    public:
        void applyEffect(juce::Image& sourceImage, juce::Graphics& destContext, float scaleFactor, float alpha) override;
    };
}
