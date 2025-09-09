//
// Created by Matt on 8/20/2025.
//

#pragma once

#include <meta/audio/OverlapAndAdd.h>


TEST(OverlapAndAddTest, overlap_3x)
{
    const auto nOverlap = 3;
    const auto buffSize = 6;
    meta::OverlapAndAdd<float> oaa(2, buffSize, nOverlap, 12);
    juce::AudioBuffer<float> inBuff(2, buffSize / 2);

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

    for (int i = 5; --i >= 0;)
    {
        x.makeCopyOf(inBuff);

        oaa.process(x);
        ASSERT_EQ(0, 0);
    }
}