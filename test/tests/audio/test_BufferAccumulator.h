//
// Created by Matt on 8/15/2025.
//

#pragma once

#include "inc/meta/audio/BufferAccumulator.h"


TEST(BufferAccumulatorTest, accumulate_exact)
{
    int hit = 0;
    juce::AudioBuffer<float> in(2, 105);
    meta::BufferAccumulator<float> accumulator(2, 10);
    accumulator.onComplete = [&](juce::AudioBuffer<float> x){
        hit++;
        ASSERT_EQ(x.getNumSamples(), 10);
    };

    accumulator.accumulate(in);

    ASSERT_EQ(hit, 10);
}