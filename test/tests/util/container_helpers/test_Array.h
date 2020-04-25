//
// Created by Matt Zapp on 3/22/2020.
//

#pragma once
#include <meta/util/container_helpers/array.h>

TEST(ArrayHelpersTest, concatenate)
{
	constexpr std::array<int, 3> x{ 1, 2, 3 };
	constexpr std::array<int, 3> y{ 4, 5, 6 }; 
	std::array<int, 6> test{ 1, 2, 3, 4, 5, 6 };
	constexpr auto concated = meta::concat_array(x, y);
	EXPECT_EQ(concated, test);
}


TEST(ArrayHelpersTest, negate)
{
	constexpr std::array<int, 3> x{ 1, 2, 3 };
	constexpr std::array<int, 3> test{ -1, -2, -3 };
	constexpr auto negated = meta::negate_array(x);
	ASSERT_EQ(negated, test);
}

