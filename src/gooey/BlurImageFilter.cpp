//
// Created by Matt on 12/1/2024.
//

#include "meta/gooey/BlurImageFilter.h"


BlurImageFilter::BlurImageFilter(int kernelSize)
    : m_Kernel(kernelSize)
{}

void BlurImageFilter::applyEffect(juce::Image& sourceImage, juce::Graphics& destContext, float scaleFactor, float alpha)
{
    juce::Image tmp(sourceImage.getFormat(), sourceImage.getWidth(), sourceImage.getHeight(), false);
    m_Kernel.applyToImage(tmp, sourceImage, sourceImage.getBounds());
}
