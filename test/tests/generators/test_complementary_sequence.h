//
// Created by Matt Zapp on 3/31/2020.
//
#pragma once
#include <meta/generators/complementary_sequence.h>

#define GOLAY_N 15
TEST(GolayTest, generate_sequence)
{

//	constexpr std::array<char, 4> = { 1, 1, 1, -1 };
//	constexpr std::array<char, 4> b = { 1, 1, -1, 1 };
	meta::complementary_sequence<meta::Golay<GOLAY_N>::size> golay = meta::Golay<GOLAY_N>::generate();
	ASSERT_EQ((meta::Golay<2>::size), 4);
	ASSERT_EQ(std::get<0>(golay)[0], 1);
//	ASSERT_EQ(std::get<0>(golay), a);
//    ASSERT_EQ(std::get<1>(golay), b);
}