//
// Created by Matt on 8/20/2025.
//

#pragma once

#include <meta/audio/OverlapAndAdd.h>
#include <meta/testing/TestHelpers.h>


TEST(OverlapAndAddTest, overlap_4x)
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

    oaa.processChunk = [](juce::AudioBuffer<float>&) {};

    juce::AudioBuffer<float> x(2, inBuff.getNumSamples());

    // Run enough iterations to get past latency and into steady state
    for (int i = 20; --i >= 0;)
    {
        x.makeCopyOf(inBuff);
        oaa.process(x);
    }

    // After many iterations of a constant DC=1 signal on channel 1,
    // overlap-add with identity callback should reconstruct to 1.0
    for (int s = 0; s < x.getNumSamples(); s++)
    {
        ASSERT_NEAR(x.getArrayOfWritePointers()[1][s], 1.0f, 1e-5f);
    }
}

TEST(OverlapAndAddTest, identity_passthrough)
{
    const int nSamps = 8;
    const int nOverlap = 2;
    const int blockSize = 4;
    meta::OverlapAndAdd<float> oaa(1, nSamps, nOverlap, blockSize);

    oaa.processChunk = [](juce::AudioBuffer<float>&) {};

    juce::AudioBuffer<float> block(1, blockSize);

    // Fill with DC = 1.0
    for (int s = 0; s < blockSize; s++) { block.getArrayOfWritePointers()[0][s] = 1.0f; }

    // First nSamps samples are latency (zeros)
    int totalSamples = 0;
    for (int i = 0; i < nSamps / blockSize; i++)
    {
        juce::AudioBuffer<float> x(1, blockSize);
        x.makeCopyOf(block);
        oaa.process(x);
        for (int s = 0; s < blockSize; s++)
        {
            ASSERT_NEAR(x.getArrayOfWritePointers()[0][s], 0.0f, 1e-5f)
                << "Expected zero during latency at sample " << totalSamples + s;
        }
        totalSamples += blockSize;
    }

    // Ramp-up: (nOverlap - 1) strides where not all windows overlap yet
    const int rampUpBlocks = (nOverlap - 1) * (nSamps / nOverlap) / blockSize;
    for (int i = 0; i < rampUpBlocks; i++)
    {
        juce::AudioBuffer<float> x(1, blockSize);
        x.makeCopyOf(block);
        oaa.process(x);
        totalSamples += blockSize;
    }

    // After latency + ramp-up, output should reconstruct input DC = 1.0
    for (int i = 0; i < 10; i++)
    {
        juce::AudioBuffer<float> x(1, blockSize);
        x.makeCopyOf(block);
        oaa.process(x);
        for (int s = 0; s < blockSize; s++)
        {
            ASSERT_NEAR(x.getArrayOfWritePointers()[0][s], 1.0f, 1e-5f)
                << "Expected 1.0 after steady state at sample " << totalSamples + s;
        }
        totalSamples += blockSize;
    }
}

TEST(OverlapAndAddTest, latency_is_correct)
{
    const int nSamps = 16;
    const int nOverlap = 2;
    const int blockSize = 4;
    meta::OverlapAndAdd<float> oaa(1, nSamps, nOverlap, blockSize);

    oaa.processChunk = [](juce::AudioBuffer<float>&) {};

    ASSERT_EQ(oaa.getLatency(), nSamps);

    juce::AudioBuffer<float> block(1, blockSize);
    for (int s = 0; s < blockSize; s++) { block.getArrayOfWritePointers()[0][s] = 5.0f; }

    // First nSamps samples of output must be zero
    int totalSamples = 0;
    for (int i = 0; i < nSamps / blockSize; i++)
    {
        juce::AudioBuffer<float> x(1, blockSize);
        x.makeCopyOf(block);
        oaa.process(x);
        for (int s = 0; s < blockSize; s++)
        {
            ASSERT_NEAR(x.getArrayOfWritePointers()[0][s], 0.0f, 1e-5f)
                << "Expected zero during latency at sample " << totalSamples + s;
        }
        totalSamples += blockSize;
    }
}

TEST(OverlapAndAddTest, overlap_4x_reconstruction)
{
    const int nSamps = 16;
    const int nOverlap = 4;
    const int blockSize = 4;
    meta::OverlapAndAdd<float> oaa(1, nSamps, nOverlap, blockSize);

    oaa.processChunk = [](juce::AudioBuffer<float>&) {};

    juce::AudioBuffer<float> block(1, blockSize);
    for (int s = 0; s < blockSize; s++) { block.getArrayOfWritePointers()[0][s] = 1.0f; }

    // Skip latency period
    for (int i = 0; i < nSamps / blockSize; i++)
    {
        juce::AudioBuffer<float> x(1, blockSize);
        x.makeCopyOf(block);
        oaa.process(x);
    }

    // Skip ramp-up: (nOverlap - 1) strides
    const int rampUpBlocks = (nOverlap - 1) * (nSamps / nOverlap) / blockSize;
    for (int i = 0; i < rampUpBlocks; i++)
    {
        juce::AudioBuffer<float> x(1, blockSize);
        x.makeCopyOf(block);
        oaa.process(x);
    }

    // After latency + ramp-up, steady-state should reconstruct DC = 1.0
    for (int i = 0; i < 20; i++)
    {
        juce::AudioBuffer<float> x(1, blockSize);
        x.makeCopyOf(block);
        oaa.process(x);
        for (int s = 0; s < blockSize; s++)
        {
            ASSERT_NEAR(x.getArrayOfWritePointers()[0][s], 1.0f, 1e-5f)
                << "Reconstruction failed at iteration " << i << " sample " << s;
        }
    }
}

TEST(OverlapAndAddTest, block_equals_window_size)
{
    // nOverlap=1 means stride == nSamps (no overlap).
    // Block size == nSamps directly exercises the >= fix.
    const int nSamps = 8;
    const int nOverlap = 1;
    const int blockSize = nSamps;
    meta::OverlapAndAdd<float> oaa(1, nSamps, nOverlap, blockSize);

    oaa.processChunk = [](juce::AudioBuffer<float>&) {};

    juce::AudioBuffer<float> block(1, blockSize);
    for (int s = 0; s < blockSize; s++) { block.getArrayOfWritePointers()[0][s] = 1.0f; }

    // First block is latency
    juce::AudioBuffer<float> x(1, blockSize);
    x.makeCopyOf(block);
    oaa.process(x);
    for (int s = 0; s < blockSize; s++)
    {
        ASSERT_NEAR(x.getArrayOfWritePointers()[0][s], 0.0f, 1e-5f)
            << "Expected zero during latency at sample " << s;
    }

    // Second block should reconstruct DC = 1.0
    x.makeCopyOf(block);
    oaa.process(x);
    for (int s = 0; s < blockSize; s++)
    {
        ASSERT_NEAR(x.getArrayOfWritePointers()[0][s], 1.0f, 1e-5f)
            << "block_equals_window_size: expected 1.0 at sample " << s;
    }
}

TEST(OverlapAndAddTest, two_channel_independence)
{
    const int nSamps = 8;
    const int nOverlap = 2;
    const int blockSize = 4;
    meta::OverlapAndAdd<float> oaa(2, nSamps, nOverlap, blockSize);

    oaa.processChunk = [](juce::AudioBuffer<float>&) {};

    juce::AudioBuffer<float> block(2, blockSize);
    for (int s = 0; s < blockSize; s++)
    {
        block.getArrayOfWritePointers()[0][s] = 1.0f;
        block.getArrayOfWritePointers()[1][s] = 2.0f;
    }

    // Skip latency
    for (int i = 0; i < nSamps / blockSize; i++)
    {
        juce::AudioBuffer<float> x(2, blockSize);
        x.makeCopyOf(block);
        oaa.process(x);
    }

    // Skip ramp-up: (nOverlap - 1) strides
    const int rampUpBlocks = (nOverlap - 1) * (nSamps / nOverlap) / blockSize;
    for (int i = 0; i < rampUpBlocks; i++)
    {
        juce::AudioBuffer<float> x(2, blockSize);
        x.makeCopyOf(block);
        oaa.process(x);
    }

    // After latency + ramp-up, verify each channel reconstructs independently
    for (int i = 0; i < 10; i++)
    {
        juce::AudioBuffer<float> x(2, blockSize);
        x.makeCopyOf(block);
        oaa.process(x);
        for (int s = 0; s < blockSize; s++)
        {
            ASSERT_NEAR(x.getArrayOfWritePointers()[0][s], 1.0f, 1e-5f)
                << "Channel 0 failed at iteration " << i << " sample " << s;
            ASSERT_NEAR(x.getArrayOfWritePointers()[1][s], 2.0f, 1e-5f)
                << "Channel 1 failed at iteration " << i << " sample " << s;
        }
    }
}
