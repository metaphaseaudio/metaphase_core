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

TEST(BufferBufferTest, full_loop_values)
{
    // Push 10 samples [0..9], pop 5, push those 5 back, pop 5 more.
    // First pop should yield [0,1,2,3,4]; second should yield [0,1,2,3,4] (the re-pushed samples).
    juce::AudioBuffer<float> in(1, 10);
    juce::AudioBuffer<float> out(1, 5);

    for (int i = 0; i < 10; ++i) { in.getArrayOfWritePointers()[0][i] = static_cast<float>(i); }

    meta::BufferBuffer<float> bufferBuffer(1, 11);
    bufferBuffer.push(in);
    ASSERT_EQ(bufferBuffer.getFreeSpace(), 0);

    bufferBuffer.pop(out);
    ASSERT_TRUE(meta::TestHelpers::bufferIsEqual(out, {{0, 1, 2, 3, 4}}));

    bufferBuffer.push(out);
    bufferBuffer.pop(out);
    ASSERT_TRUE(meta::TestHelpers::bufferIsEqual(out, {{5, 6, 7, 8, 9}}));
}

TEST(BufferBufferTest, push_pop_wrap_boundary)
{
    // Use a small buffer so push/pop heads wrap around the physical boundary.
    // Buffer capacity = 5. Push 4, pop 3 (move read head to 3), push 4 more (wraps), pop 4.
    meta::BufferBuffer<float> bb(1, 5);

    auto make = [](std::initializer_list<float> vals)
    {
        juce::AudioBuffer<float> b(1, static_cast<int>(vals.size()));
        int i = 0;
        for (float v : vals) { b.getArrayOfWritePointers()[0][i++] = v; }
        return b;
    };

    auto a = make({10, 20, 30, 40});
    bb.push(a);

    auto discard = make({0, 0, 0});
    bb.pop(discard); // consume 3, read head now at 3

    auto b = make({1, 2, 3, 4}); // write will wrap around
    bb.push(b);

    auto out = make({0, 0, 0, 0});
    bb.pop(out);
    ASSERT_TRUE(meta::TestHelpers::bufferIsEqual(out, {{1, 2, 3, 4}}));
}

TEST(BufferBufferTest, peek_no_offset)
{
    juce::AudioBuffer<float> in(1, 5);
    for (int i = 0; i < 5; ++i) { in.getArrayOfWritePointers()[0][i] = static_cast<float>(i + 1); }

    meta::BufferBuffer<float> bb(1, 10);
    bb.push(in);

    juce::AudioBuffer<float> peeked(1, 5);
    bb.peek(peeked); // default offset = 0

    ASSERT_TRUE(meta::TestHelpers::bufferIsEqual(peeked, {{1, 2, 3, 4, 5}}));

    // Read head must not have advanced — pop should return the same data.
    juce::AudioBuffer<float> popped(1, 5);
    bb.pop(popped);
    ASSERT_TRUE(meta::TestHelpers::bufferIsEqual(popped, {{1, 2, 3, 4, 5}}));
}

TEST(BufferBufferTest, peek_with_offset)
{
    juce::AudioBuffer<float> in(1, 8);
    for (int i = 0; i < 8; ++i) { in.getArrayOfWritePointers()[0][i] = static_cast<float>(i); }

    meta::BufferBuffer<float> bb(1, 10);
    bb.push(in);

    // Skip first 3 samples, peek at next 4.
    juce::AudioBuffer<float> peeked(1, 4);
    bb.peek(peeked, 3);
    ASSERT_TRUE(meta::TestHelpers::bufferIsEqual(peeked, {{3, 4, 5, 6}}));

    // Read head must still be at the start.
    ASSERT_EQ(bb.getNumSamplesReady(), 8);
}

TEST(BufferBufferTest, drop_samples)
{
    juce::AudioBuffer<float> in(1, 8);
    for (int i = 0; i < 8; ++i) { in.getArrayOfWritePointers()[0][i] = static_cast<float>(i); }

    meta::BufferBuffer<float> bb(1, 10);
    bb.push(in);

    bb.drop(3); // discard [0,1,2]

    juce::AudioBuffer<float> out(1, 5);
    bb.pop(out);
    ASSERT_TRUE(meta::TestHelpers::bufferIsEqual(out, {{3, 4, 5, 6, 7}}));
}

TEST(BufferBufferTest, transfer_from_other)
{
    juce::AudioBuffer<float> in(1, 6);
    for (int i = 0; i < 6; ++i) { in.getArrayOfWritePointers()[0][i] = static_cast<float>(i + 1); }

    meta::BufferBuffer<float> src(1, 10);
    src.push(in);

    meta::BufferBuffer<float> dst(1, 10);
    dst.transferFromOther(src, 6);

    ASSERT_EQ(src.getNumSamplesReady(), 0);
    ASSERT_EQ(dst.getNumSamplesReady(), 6);

    juce::AudioBuffer<float> out(1, 6);
    dst.pop(out);
    ASSERT_TRUE(meta::TestHelpers::bufferIsEqual(out, {{1, 2, 3, 4, 5, 6}}));
}

TEST(BufferBufferTest, multi_channel_push_pop)
{
    // 2 channels with distinct per-channel values.
    juce::AudioBuffer<float> in(2, 4);
    for (int i = 0; i < 4; ++i)
    {
        in.getArrayOfWritePointers()[0][i] = static_cast<float>(i + 1);      // ch0: 1,2,3,4
        in.getArrayOfWritePointers()[1][i] = static_cast<float>((i + 1) * 10); // ch1: 10,20,30,40
    }

    meta::BufferBuffer<float> bb(2, 8);
    bb.push(in);

    juce::AudioBuffer<float> out(2, 4);
    bb.pop(out);

    ASSERT_TRUE(meta::TestHelpers::bufferIsEqual(out, {{1, 2, 3, 4}, {10, 20, 30, 40}}));
}