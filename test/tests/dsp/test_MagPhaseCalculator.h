//
// Created by Matt on 7/26/2021.
//

#pragma once
#include <inc/meta/dsp/BandlimitedWavetable.h>
#include <inc/meta/dsp/WavetableReader.h>


class MagPhaseCalculatorTest
    : public testing::Test
{
public:
    static constexpr float SAMPLE_RATE = 48000;
    static constexpr int FFT_SIZE = 1024;

    MagPhaseCalculatorTest()
        : osc(table, state)
    { osc.setFrequency(SAMPLE_RATE, 0); };

    void init_signal_buffer(int samples)
    {
        buffer.setSize(1, samples);
        buffer.clear();
        for (int i = 0; i < samples; i++)
        {
            auto sample = osc.tick();
            buffer.setSample(0,i, sample);
        }
    }

    std::array<float, 1000> table = meta::BandlimitedWavetable<float, 1000>::makeSin();
    meta::WavetableState<float> state;
    meta::WavetableReader<float, 1000> osc;
    juce::AudioBuffer<float> buffer;

};


TEST_F(MagPhaseCalculatorTest, calculate)
{
    osc.setFrequency(SAMPLE_RATE, 100);
    init_signal_buffer(10000);

    juce::dsp::FFT fft(10);
    juce::dsp::WindowingFunction<float> window(FFT_SIZE, juce::dsp::WindowingFunction<float>::hann);
    juce::AudioBuffer<float> test_buffer(1, FFT_SIZE * 2);
    test_buffer.clear();
    test_buffer.copyFrom(0,0, buffer, 0, 0, FFT_SIZE);
    window.multiplyWithWindowingTable(test_buffer.getWritePointer(0), FFT_SIZE);

    std::array<std::complex<float>, FFT_SIZE> in, out;

    for (int i = FFT_SIZE; --i >=0;) { in.at(i) = test_buffer.getSample(0, i); }


    fft.perform(in.data(), out.data(), false);

    std::array<float, FFT_SIZE> mag, phase;
    for (int i = FFT_SIZE; --i >=0;)
    {
        mag.at(i) = std::abs(out.at(i));
        phase.at(i) = std::abs(out.at(i)) <= std::numeric_limits<float>::epsilon() ? 0 : std::atan2(out.at(i).imag(), out.at(i).real());
    }

    auto argmax = meta::argmax(mag.begin(), mag.end());
    auto distance = std::distance(mag.begin(), argmax);
    ASSERT_EQ(distance, 100);
}
