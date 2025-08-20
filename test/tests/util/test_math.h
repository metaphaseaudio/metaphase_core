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
    constexpr auto value = meta::static_power<2, 2>::value;
    static_assert(meta::static_power<2, 2>::value == 4, "meta::static_power compiler failure: 2^2 != 4");
    static_assert(meta::static_power<2, 3>::value == 8, "meta::static_power compiler failure: 2^2 != 4");
    static_assert(meta::static_power<2, 12>::value == 4096, "meta::static_power compiler failure: 2^2 != 4");

}

TEST(MetaMath, static_negate)
{
    constexpr auto value = meta::static_negate<int, 2>::value;
    ASSERT_EQ(value, -2);
}
