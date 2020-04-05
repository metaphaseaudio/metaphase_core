//
// Created by Matt Zapp on 3/31/2020.
//
#pragma once
#include <meta/util/vectorized_math.h>

TEST(MetaVecops, pow_2)
{
	float in[] = { 1, 2, 3, 4, 5 };
	float out[] { 0, 0, 0, 0, 0 };
	float test[] = { 1, 4, 9, 16, 35 };

	meta::vecops<float>::pow(in, out, 5, 2);
	ASSERT_EQ(out, test);
}
