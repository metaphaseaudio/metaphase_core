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

TEST(BufferBufferTest, add_to_empty_buffer_with_offset)
{
    juce::AudioBuffer<float> in(1, 5);
    juce::AudioBuffer<float> out(1, 10);

    for (int i = in.getNumSamples(); --i >= 0;) { in.getArrayOfWritePointers()[0][i] = static_cast<float>(i); }

    meta::BufferBuffer<float> bufferBuffer(1, 20);

    bufferBuffer.addAtOffsetFromReadHead(in, 5);

    bufferBuffer.pop(out);

    ASSERT_TRUE(meta::TestHelpers::bufferIsEqual(out, {{0,0,0,0,0,0,1,2,3,4}}));
}

TEST(BufferBufferTest, add_with_offset)
{
    juce::AudioBuffer<float> in(1, 5);
    juce::AudioBuffer<float> out(1, 7);

    for (int i = in.getNumSamples(); --i >= 0;) { in.getArrayOfWritePointers()[0][i] = static_cast<float>(i); }

    meta::BufferBuffer<float> bufferBuffer(1, 20);
    bufferBuffer.pushZeros(7);
    bufferBuffer.addAtOffsetFromReadHead(in, 2);

    ASSERT_EQ(bufferBuffer.getNumSamplesReady(), 7);

    bufferBuffer.pop(out);

    ASSERT_TRUE(meta::TestHelpers::bufferIsEqual(out, {{0, 0, 0, 1, 2, 3, 4}}));
}

TEST(BufferBufferTest, add_with_offset_across_loop_boundary)
{
    juce::AudioBuffer<float> in(1, 5);
    juce::AudioBuffer<float> out(1, 7);

    for (int i = in.getNumSamples(); --i >= 0;) { in.getArrayOfWritePointers()[0][i] = static_cast<float>(i); }

    meta::BufferBuffer<float> bufferBuffer(1, 7);

    // move the read head
    bufferBuffer.pushZeros(2);
    auto tmp = juce::AudioBuffer<float>(1,2);
    bufferBuffer.pop(tmp);


    bufferBuffer.pushZeros(7);
    bufferBuffer.addAtOffsetFromReadHead(in, 2);

    ASSERT_EQ(bufferBuffer.getNumSamplesReady(), 7);

    bufferBuffer.pop(out);

    ASSERT_TRUE(meta::TestHelpers::bufferIsEqual(out, {{0, 0, 0, 1, 2, 3, 4}}));
}