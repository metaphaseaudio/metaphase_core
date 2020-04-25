//
// Created by Matt Zapp on 3/31/2020.
//
#pragma once
#include <meta/util/math.h>

TEST(MetaMath, limit)
{
	ASSERT_EQ(meta::limit(0.0, 1.0, 0.5), 0.5);
	ASSERT_EQ(meta::limit(0.0, 1.0, 2.0), 1);
	ASSERT_EQ(meta::limit(0.0, 1.0, -1.0), 0);
}

TEST(MetaMath, power)
{
	ASSERT_EQ(meta::power(2, 2), 4);
}


TEST(MetaMath, static_power)
{
    ASSERT_EQ((meta::static_power<2, 2>::value), 4);
}

TEST(MetaMath, static_negate)
{
    ASSERT_EQ((meta::static_negate<int, 2>::value), -2);
}
