//
// Created by Matt on 7/7/2021.
//

#pragma once
#include <meta/dsp/MultiChanConvolve.h>


TEST(MultiChanConvolveTest, two_chan_first_chan_delay)
{
    juce::AudioBuffer<float> impulse(2, 4);
    impulse.clear();
    impulse.setSample(0, 1, 1.0f);  // this should introduce a delay
    impulse.setSample(1, 0, 1.0f);  // this should not

    juce::AudioBuffer<float> signal(2, 5);
    signal.clear();
    signal.setSample(0, 4, 1.0f);
    signal.setSample(1, 2, 1.0f);
    juce::AudioBuffer<float> output(2, impulse.getNumSamples() + signal.getNumSamples());

    meta::dsp::MultiChanConvolve convolve(std::move(impulse), 3);
    output = convolve.convolve(signal, true);
    ASSERT_EQ(output.getSample(0, 4), 0.0f);
    ASSERT_EQ(output.getSample(0, 5), 1.0f);
    ASSERT_EQ(output.getSample(1, 1), 0.0f);
    ASSERT_EQ(output.getSample(1, 2), 1.0f);
}