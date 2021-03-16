//
// Created by Matt Zapp on 3/31/2020.
//
#pragma once
#include <meta/util/container_helpers/array.h>
#include <meta/generators/range_sequence.h>

TEST(RangeSequenceTest, integer_sequence)
{
	constexpr auto a = meta::range_sequence<int, 0, 10, 2>::value;
	ASSERT_EQ(a.size(), 5);
	for (auto item : meta::enumerate(a))
    {
        std::cout << item.first << item.second << std::endl;
    }
}
