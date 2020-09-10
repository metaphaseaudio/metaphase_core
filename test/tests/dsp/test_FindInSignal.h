//
// Created by Matt on 8/1/2020.
//
#pragma once
#include <meta/generators/complementary_sequence.h>
#include <meta/dsp/FindInSignal.h>

#define GOLAY_N 15


TEST(FindInSignalTest, find_golay)
{
    constexpr meta::complementary_sequence<meta::Golay<GOLAY_N>::size> golay = meta::Golay<GOLAY_N>::value;

    ASSERT_EQ((meta::Golay<GOLAY_N>::size), std::pow(2, GOLAY_N));
    //ASSERT_EQ(std::get<0>(golay)[0], 1);
    //ASSERT_EQ(std::get<0>(golay), a);
    //ASSERT_EQ(std::get<1>(golay), b);
}
