//
// Created by Matt Zapp on 3/31/2020.
//
#pragma once
#include <meta/generators/Golay.h>


TEST(GolayTest, generate_sequence)
{
	constexpr std::array<char, 4> a = { 1, 1, 1, -1 };
	constexpr std::array<char, 4> b = { 1, 1, -1, 1 };
	auto golay = meta::Golay<2>::generate();
	ASSERT_EQ(std::get<0>(golay), a);
	ASSERT_EQ(std::get<1>(golay), b);
}