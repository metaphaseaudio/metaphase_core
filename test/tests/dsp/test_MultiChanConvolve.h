//
// Created by Matt on 7/7/2021.
//

#pragma once
#include <meta/dsp/MultiChanConvolve.h>


TEST(MultiChanConvolveTest, introduce_delay)
{
    juce::AudioBuffer<float> impulse(1, 2);
    impulse.clear();
    impulse.setSample(0, 1, 1.0f);  // this should introduce a delay

    juce::AudioBuffer<float> signal(1, 5);
    signal.clear();
    signal.setSample(0, 4, 1.0f);
    juce::AudioBuffer<float> output(1, impulse.getNumSamples() + signal.getNumSamples());

    meta::dsp::MultiChanConvolve convolve(std::move(impulse), 512);
    convolve.process_block(signal, output);
    ASSERT_EQ(output.getSample(0, 5), 1.0f);
}