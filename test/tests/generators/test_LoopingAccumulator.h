//
// Created by Matt on 3/6/2022.
//

#pragma once


#include <meta/audio/LoopingAccumulator.h>
#include "meta/testing/TestBase.h"
#include "meta/testing/TestHelpers.h"

class LoopingAccumulatorTest
        : public meta::TestBase
{
public:
    LoopingAccumulatorTest() {};

    void initializeTestFile(const juce::String& f)
    {
        meta::TestBase::initializeTestFile(meta::TestHelpers::testFolder.getChildFile(f), 48000);
    }
};


TEST_F(LoopingAccumulatorTest, one_tick)
{
    LoopingAccumulatorTest::initializeTestFile("looping_accum.wav");
    int nSamps = 48000;
    auto accum = meta::LoopingAccumulator(-1, 1, 48000, 24000);
    auto buffer = juce::AudioBuffer<float>(2, nSamps);

    for (int i = nSamps; --i >= 0;)
        { buffer.setSample(0, i, accum.tick()); }

    m_Writer->writeFromAudioSampleBuffer(buffer, 0 , nSamps);
}