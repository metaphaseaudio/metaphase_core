//
// Created by Matt Zapp on 3/22/2020.
//

#pragma once
#include <meta/util/container_helpers/array.h>

struct static_inits
{
    static constexpr auto negated = meta::negate_array(std::array<int, 3>({1, 2, 3}));
    static constexpr auto concated = meta::concat_array(std::array<int, 3>({1, 2, 3}), std::array<int, 3>({4, 5, 6}));
};

TEST(ArrayHelpersTest, concatenate)
{
	std::array<int, 3> x{ 1, 2, 3 };
	std::array<int, 3> y{ 4, 5, 6 }; 
	std::array<int, 6> test{ 1, 2, 3, 4, 5, 6 };
	auto concated = meta::concat_array(x, y);
	EXPECT_EQ(concated, test);
    EXPECT_EQ(static_inits::concated, test);
}


TEST(ArrayHelpersTest, negate)
{
	constexpr std::array<int, 3> x{ 1, 2, 3 };
	constexpr std::array<int, 3> test{ -1, -2, -3 };
	constexpr auto negated = meta::negate_array(x);
	ASSERT_EQ(negated, test);
    ASSERT_EQ(static_inits::negated, test);
}

