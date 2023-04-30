//
// Created by Matt on 4/28/2023.
//
#include "meta/gooey/PerspectiveImageEffectFilter.h"


void meta::PerspectiveImageEffectFilter::applyEffect(juce::Image& sourceImage, juce::Graphics& destContext, float scaleFactor, float alpha)
{
    sourceImage.desaturate();
    destContext.drawImageAt(sourceImage, 0, 0);
}
