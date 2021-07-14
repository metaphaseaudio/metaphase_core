//
// Created by Matt on 8/1/2020.
//
#pragma once
#include <meta/generators/complementary_sequence.h>
#include <meta/dsp/FindInSignal.h>

#define GOLAY_N 2
#define PAUSE_SAMPS 10


TEST(FindInSignalTest, find_golay)
{
    constexpr meta::complementary_sequence<meta::Golay<GOLAY_N>::size> golay = meta::Golay<GOLAY_N>::value;
    constexpr auto a = std::get<0>(golay);
    constexpr auto b = std::get<1>(golay);

    juce::AudioBuffer<float> test_signal(1, PAUSE_SAMPS + a.size() + PAUSE_SAMPS + b.size() + PAUSE_SAMPS);
    juce::AudioBuffer<float> burst_a(1, a.size());
    juce::AudioBuffer<float> burst_b(1, b.size());

    test_signal.clear();
    auto samp_i = PAUSE_SAMPS;
    auto burst_i = 0;
    for (const auto& samp : a)
    {
        test_signal.setSample(0, samp_i, samp); samp_i++;
        burst_a.setSample(0, burst_i, samp); burst_i++;
    }

    samp_i += PAUSE_SAMPS;
    burst_i = 0;
    for (const auto& samp : b)
    {
        test_signal.setSample(0, samp_i, samp); samp_i++;
        burst_b.setSample(0, burst_i, samp); burst_i++;
    }

    auto range_pair_a = meta::dsp::find_in_signal(burst_a, test_signal);
    auto range_pair_b = meta::dsp::find_in_signal(burst_b, test_signal);

    ASSERT_EQ(range_pair_a.first, PAUSE_SAMPS);
    ASSERT_EQ(range_pair_a.second, PAUSE_SAMPS + a.size() - 1);

    ASSERT_EQ(range_pair_b.first, (PAUSE_SAMPS * 2) + a.size());
    ASSERT_EQ(range_pair_b.second, (PAUSE_SAMPS * 2) + a.size() + b.size() - 1);
}
