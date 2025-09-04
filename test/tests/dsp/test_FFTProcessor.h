#pragma once
#include <meta/dsp/BandlimitedWavetable.h>
#include <meta/dsp/WavetableReader.h>
#include <meta/dsp/FFTProcessor.h>
#include <valarray>


class FFTCalculatorTest
    : public testing::Test
{
public:
    static constexpr float SAMPLE_RATE = 48000;

    FFTCalculatorTest()
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


TEST_F(FFTCalculatorTest, calculate)
{
    using FFT = meta::dsp::FFTProcessor<float, 4, juce::dsp::WindowingFunction<float>::WindowingMethod::hamming>;
    osc.setFrequency(SAMPLE_RATE, 1000);
    init_signal_buffer(FFT::FFTSize);
    juce::AudioBuffer<float> test_buffer(1, FFT::FFTSize);
    test_buffer.clear();
    test_buffer.copyFrom(0,0, buffer, 0, 0, FFT::FFTSize);

    FFT calc;
    FFT::FFTFrame frame = {{0}};
    calc.process(test_buffer, frame);

    test_buffer.clear();
    calc.inverse(frame, test_buffer);

    auto distance = 21;
    ASSERT_EQ(distance, 21);
}

