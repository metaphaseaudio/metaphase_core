//
// Created by Matt on 4/30/2022.
//

#pragma once

#include <meta/dsp/OversampledBuffer.h>

#define OS_BUFFER_SAMPLE_RATE 48000
#define OS_COUNT 128

class OSBufferTest
    : public meta::TestBase
{
public:
    OSBufferTest()
        : buff(OS_BUFFER_SAMPLE_RATE)
    {};

    void initializeTestFile(const juce::String& f)
        { meta::TestBase::initializeTestFile(meta::TestHelpers::testFolder.getChildFile(f)); }

    meta::OversampledBuffer<16, OS_COUNT, 12, 2> buff;
};


TEST_F(OSBufferTest, generate_transition)
{
    initializeTestFile("os_buff_test.wav");

    const int sampCount = 50000;
    juce::AudioBuffer<float> outDat(2, sampCount);
    juce::AudioBuffer<float> inDat(2, sampCount * buff.OverSample);
    inDat.clear();
    outDat.clear();

    const int rampEnd = (sampCount * buff.OverSample) / 2;
    for (int s = 0; s < rampEnd; s++)
    {
        const auto v = (s * 2.0f - rampEnd) / rampEnd;
        inDat.setSample(0, s, v);
        inDat.setSample(1, s, -v);
    }

    buff.downsampleBuffer(inDat, outDat);

    m_Writer->writeFromAudioSampleBuffer(outDat, 0, outDat.getNumSamples());
    m_Writer->flush();
}
