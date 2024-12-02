//
// Created by Matt on 12/1/2024.
//

#pragma once

#include <juce_graphics/juce_graphics.h>

class BlurImageFilter
    : public juce::ImageEffectFilter
{
public:
    explicit BlurImageFilter(int kernelSize);

    void applyEffect(juce::Image& sourceImage, juce::Graphics& destContext, float scaleFactor, float alpha) override;

private:
    juce::ImageConvolutionKernel m_Kernel;
};
