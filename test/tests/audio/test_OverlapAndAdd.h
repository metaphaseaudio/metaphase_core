//
// Created by Matt on 8/20/2025.
//

#pragma once

#include <meta/audio/OverlapAndAdd.h>


TEST(OverlapAndAddTest, overlap_3x)
{
    const auto nOverlap = 4;
    const auto buffSize = 16;
    meta::OverlapAndAdd<float> oaa(2, buffSize, nOverlap, 5);
    juce::AudioBuffer<float> inBuff(2, 5);

    inBuff.clear();

    inBuff.getArrayOfWritePointers()[0][0] = -1;

    for (int s = 0; s < inBuff.getNumSamples(); s++)
    {
        inBuff.getArrayOfWritePointers()[1][s] = 1;
    }

    oaa.processChunk = [](auto)
    {
    };

    juce::AudioBuffer<float> x(2, inBuff.getNumSamples());

    for (int i = 20; --i >= 0;)
    {
        x.makeCopyOf(inBuff);

        oaa.process(x);
        ASSERT_EQ(0, 0);
    }
}