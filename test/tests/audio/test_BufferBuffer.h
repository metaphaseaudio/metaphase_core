//
// Created by Matt on 8/19/2025.
//

#pragma once

#include <meta/audio/BufferBuffer.h>


TEST(BufferBufferTest, full_loop)
{
    juce::AudioBuffer<float> in(1, 11);
    juce::AudioBuffer<float> out(1, 5);

    for (int i = in.getNumSamples(); --i >= 0;) { in.getArrayOfWritePointers()[0][i] = static_cast<float>(i); }

    meta::BufferBuffer<float> bufferBuffer(1, 11);

    bufferBuffer.push(in);

    ASSERT_EQ(bufferBuffer.getFreeSpace(), 0);

    bufferBuffer.pop(out);
    bufferBuffer.push(out);
    bufferBuffer.pop(out);
}