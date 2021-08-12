//
// Created by Matt on 7/26/2021.
//

#pragma once
#include <meta/dsp/BandlimitedWavetable.h>
#include <meta/dsp/WavetableReader.h>
#include <meta/dsp/MagPhaseCalculator.h>


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
    osc.setFrequency(SAMPLE_RATE, 1000);
    init_signal_buffer(1050);
    juce::AudioBuffer<float> test_buffer(1, FFT_SIZE * 2);
    test_buffer.clear();
    test_buffer.copyFrom(0,0, buffer, 0, 0, FFT_SIZE);
    meta::dsp::MagPhaseCalculator<float> calc(10);
    auto result = calc.calculate_window(test_buffer, 0, 0);
    auto magnitude = std::get<0>(result);

    auto argmax = meta::argmax(magnitude.begin(), magnitude.end());
    auto distance = std::distance(magnitude.begin(), argmax);
    ASSERT_EQ(distance, 21);
}
