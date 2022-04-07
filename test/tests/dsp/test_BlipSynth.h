//
// Created by Matt Zapp on 3/15/2020.
//

#pragma once

#include <meta/testing/TestBase.h>
#include <meta/testing/TestHelpers.h>
#include <meta/util/file/AudioFileHelpers.h>
#include <meta/dsp/BandlimitedOsc.h>


#define BLIP_OSC_SAMPLE_RATE 48000


class BlipSynthTest
    : public meta::TestBase
{
public:
    BlipSynthTest()
        : osc(BLIP_OSC_SAMPLE_RATE)
    {};


    void initializeTestFile(const juce::String& f)
    {
        meta::TestBase::initializeTestFile(meta::TestHelpers::testFolder.getChildFile(f));
    }

    void runOscillator(int samples)
    {
        constexpr int chunk_size = 528;
        for (int mod = samples / chunk_size; --mod >= 0;)
        {
            const auto n_samps = std::min<int>(chunk_size, samples);

            juce::AudioBuffer<float> buffer(2, n_samps);
            buffer.clear();
            osc.processBlock(buffer.getArrayOfWritePointers(), n_samps);
            m_Writer->writeFromAudioSampleBuffer(buffer, 0, buffer.getNumSamples());
            samples -= chunk_size;
        }
    }

    meta::BandLimitedOsc<16, 128, 12, 2> osc;
};


TEST_F(BlipSynthTest, generate)
{
    initializeTestFile("blip_osc.wav");
    osc.set_freq(125);
    float n_seconds = 2;
    runOscillator(BLIP_OSC_SAMPLE_RATE * n_seconds);
}
