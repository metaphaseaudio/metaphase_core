//
// Created by Matt Zapp on 3/15/2020.
//

#pragma once

#include <meta/testing/TestBase.h>
#include <meta/testing/TestHelpers.h>
#include <meta/util/file/AudioFileHelpers.h>
#include <meta/dsp/WavetableReader.h>
#include <meta/dsp/BandlimitedWavetable.h>


class WavetableOscillatorTest
    : public meta::TestBase
{
public:
    WavetableOscillatorTest()
        : osc(table, state)
    { osc.setFrequency(0, 48000); };

    void initializeTestFile(const juce::String& f)
    {
        meta::TestBase::initializeTestFile(meta::TestHelpers::testFolder.getChildFile(f));
    }

    void runOscillator(int samples)
    {
        constexpr int chunk_size = 10000;
        for (int mod = samples / chunk_size; --mod >= 0;)
        {
            const auto n_samps = std::min<int>(chunk_size, samples);
            juce::AudioBuffer<float> buffer(1, n_samps);
            buffer.clear();
            for (int i = 0; i < n_samps; i++)
            {
                auto sample = osc.tick();
                buffer.setSample(0,i, sample);
            }
            samples -= chunk_size;
            m_Writer->writeFromAudioSampleBuffer(buffer, 0, buffer.getNumSamples());
        }
    }

    std::array<float, 1000> table = meta::BandlimitedWavetable<float, 1000>::makeSquare(100);
    meta::WavetableState<float> state;
    meta::WavetableReader<float, 1000> osc;

};


TEST_F(WavetableOscillatorTest, generate)
{
	osc.setFrequency(48000, 100);
    initializeTestFile("wavetable.wav");
    runOscillator(10000);
}


TEST_F(WavetableOscillatorTest, generate_inverse)
{
	osc.setFrequency(48000, -100);
    initializeTestFile("inverted_wavetable.wav");
    runOscillator(10000);
}

