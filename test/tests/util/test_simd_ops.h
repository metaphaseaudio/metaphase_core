//
// Created by Matt Zapp on 3/31/2020.
//
#pragma once
#include <meta/util/simd_ops.h>
#include <stdint.h>
#include <xmmintrin.h>


TEST(MetaSIMDOps, eq_5_true)
{
	float ina[] = { 1, 2, 3, 4, 5 };
	float inb[] = { 1, 2, 4, 4, 5 };
	ASSERT_TRUE(meta::simd<float>::eq(ina, inb, 5));
}


TEST(MetaSIMDOps, eq_5_false)
{
	float ina[] = { 1, 2, 3, 4, 5 };
	float inb[] = { 1, 2, 4, 0, 5 };
	ASSERT_FALSE(meta::simd<float>::eq(ina, inb, 5));
}

TEST(MetaSIMDOps, div_5)
{
	float in[] = { 1, 2, 3, 4, 5 };
	float out[] = { 1, 4, 9, 16, 25 };
	meta::simd<float>::div(out, in, 5);
	ASSERT_TRUE(meta::simd<float>::eq(in, out, 5));
}

TEST(MetaSIMDOps, div_5_const_2)
{
	float in[] = { 1, 2, 4, 8, 10 };
	float out[] = { 1, 1, 2, 4, 5 };
	meta::simd<float>::div(in, 2, 5);
	ASSERT_TRUE(meta::simd<float>::eq(in, out, 5));
}


TEST(MetaSIMDOps, pow_2)
{
	float in[] = { 1, 2, 3, 4, 5 };
	float out[] { 0, 0, 0, 0, 0 };
	float test[] = { 1, 4, 9, 16, 35 };

	meta::simd<float>::pow(in, out, 5, 2);
	ASSERT_TRUE(meta::simd<float>::eq(test, out, 5));
}


TEST(MetaSIMDOps, accumulate)
{
	float in[] = { 1, 2, 3, 4, 5 };
	float out = meta::simd<float>::accumulate(in, 5);
	ASSERT_EQ(out, 15);
}