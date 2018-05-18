//
// Created by mzapp on 4/10/18.
//

#pragma once
#include <meta/util/fixed_point/Value.h>
#include <meta/util/fixed_point/Math.h>

class FixedPointMathTest
    : public testing::Test
{
public:
    using fp1616_t = meta::FixedPointValue<int, 16>;
    fp1616_t half = fp1616_t(0.5f);
    fp1616_t one = fp1616_t(1.0);
    fp1616_t two = fp1616_t(2);
	fp1616_t twoPointFive = fp1616_t(2.5f);
};

TEST_F(FixedPointMathTest, add_fixed_fixed) { ASSERT_EQ(1.5f, static_cast<float>(half + one)); }
TEST_F(FixedPointMathTest, add_fixed_int)   { ASSERT_EQ(1.5f, static_cast<float>(half + 1)); }
TEST_F(FixedPointMathTest, add_int_fixed)   { ASSERT_EQ(1.5f, static_cast<float>(1 + half)); }
TEST_F(FixedPointMathTest, add_fixed_float) { ASSERT_EQ(1.5f, static_cast<float>(half + 1.0f)); }
TEST_F(FixedPointMathTest, add_float_fixed) { ASSERT_EQ(1.5f, static_cast<float>(1.0f + half)); }

TEST_F(FixedPointMathTest, sub_fixed_fixed_neg) { ASSERT_EQ(-0.5f, static_cast<float>(half - one)); }
TEST_F(FixedPointMathTest, sub_fixed_fixed)     { ASSERT_EQ(0.5f, static_cast<float>(one - half)); }
TEST_F(FixedPointMathTest, sub_fixed_int)       { ASSERT_EQ(-0.5f, static_cast<float>(half - 1)); }
TEST_F(FixedPointMathTest, sub_int_fixed)       { ASSERT_EQ(0.5f, static_cast<float>(1 - half)); }
TEST_F(FixedPointMathTest, sub_fixed_float)     { ASSERT_EQ(-0.5f, static_cast<float>(half - 1.0f)); }
TEST_F(FixedPointMathTest, sub_float_fixed)     { ASSERT_EQ(0.5f, static_cast<float>(1.0f - half)); }

TEST_F(FixedPointMathTest, mult_fixed_fixed) { ASSERT_EQ(1.0f, static_cast<float>(two * half)); }
TEST_F(FixedPointMathTest, mult_fixed_int)   { ASSERT_EQ(1.0f, static_cast<float>(half * 2)); }
TEST_F(FixedPointMathTest, mult_int_fixed)   { ASSERT_EQ(1.0f, static_cast<float>(2 * half)); }
TEST_F(FixedPointMathTest, mult_fixed_float) { ASSERT_EQ(1.0f, static_cast<float>(half * 2.0f)); }
TEST_F(FixedPointMathTest, mult_float_fixed) { ASSERT_EQ(1.0f, static_cast<float>(2.0f * half)); }

TEST_F(FixedPointMathTest, div_fixed_fixed) { ASSERT_EQ(0.5f, static_cast<float>(one / two)); }

// TODO: Finish these functions
//TEST_F(FixedPointMathTest, div_fixed_int)   { ASSERT_EQ(0.5f, static_cast<float>(one / 2)); }
//TEST_F(FixedPointMathTest, div_int_fixed)   { ASSERT_EQ(0.5f, static_cast<float>(1 / two)); }
//TEST_F(FixedPointMathTest, div_fixed_float) { ASSERT_EQ(0.5f, static_cast<float>(one / 2.0f)); }
//TEST_F(FixedPointMathTest, div_float_fixed) { ASSERT_EQ(0.5f, static_cast<float>(1.0f / two)); }


TEST_F(FixedPointMathTest, abs)
{
    fp1616_t negTwoPointFive(-2.5f);
    EXPECT_EQ(2.0f, static_cast<float>(meta::abs(twoPointFive)));
    EXPECT_EQ(2.0f, static_cast<float>(meta::abs(negTwoPointFive)));
}

TEST_F(FixedPointMathTest, fabs)
{
	fp1616_t negTwoPointFive(-2.5f);
	EXPECT_EQ(2.5f, static_cast<float>(meta::fabs(twoPointFive)));
	EXPECT_EQ(2.5f, static_cast<float>(meta::fabs(negTwoPointFive)));
}