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