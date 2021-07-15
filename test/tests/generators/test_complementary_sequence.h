//
// Created by Matt Zapp on 3/31/2020.
//
#pragma once
#include <meta/generators/complementary_sequence.h>

#define GOLAY_N 15

TEST(GolayTest, generate_sequence)
{
	constexpr std::array<char, 4> a = { 1, 1, 1, -1 };
	constexpr std::array<char, 4> b = { 1, 1, -1, 1 };
	constexpr meta::complementary_sequence<meta::Golay<GOLAY_N>::size> golay = meta::Golay<GOLAY_N>::value;
	constexpr auto size = meta::Golay<GOLAY_N>::size;
	ASSERT_EQ(size, std::pow(2, GOLAY_N));

    for (int i = a.size(); --i >= 0;)
    {
        ASSERT_EQ(a[i], std::get<0>(golay)[i]);
        ASSERT_EQ(b[i], std::get<1>(golay)[i]);
    }
}

TEST(GolayTest, generate_dynamically)
{
    constexpr std::array<char, 4> a = { 1, 1, 1, -1 };
    constexpr std::array<char, 4> b = { 1, 1, -1, 1 };
    const auto test_pair = meta::generate_golay_dynamic<float>(2);

    for (int i = a.size(); --i >= 0;)
    {
        ASSERT_EQ(a[i], test_pair.first[i]);
        ASSERT_EQ(b[i], test_pair.second[i]);
    }
}
