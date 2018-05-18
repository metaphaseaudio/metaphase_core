//
// Created by mzapp on 4/10/18.
//

#pragma once
#include <meta/util/fixed_point/Value.h>
#include <meta/util/fixed_point/Math.h>

TEST(FixedPointValueTest, min_signed)
{
    EXPECT_EQ(-8.0f, static_cast<float>(meta::FixedPointValue<int8_t, 4>::minSigned()));
}
TEST(FixedPointValueTest, max_signed)
{
    EXPECT_EQ(7.9375f, static_cast<float>(meta::FixedPointValue<int8_t, 4>::maxSigned()));
}

TEST(FixedPointValueTest, convert_float_to_fixed)
{
    EXPECT_EQ(2, (meta::FixedPointValue<int, 2>(0.5f).raw()));
    EXPECT_EQ(4, (meta::FixedPointValue<int, 3>(0.5f).raw()));
    EXPECT_EQ(8, (meta::FixedPointValue<int, 4>(0.5f).raw()));
}

TEST(FixedPointValueTest, convert_fixed_to_float)
{
    EXPECT_EQ(0.5f, static_cast<float>(meta::FixedPointValue<int, 2>::fromRaw(2)));
    EXPECT_EQ(0.5f, static_cast<float>(meta::FixedPointValue<int, 3>::fromRaw(4)));
    EXPECT_EQ(0.5f, static_cast<float>(meta::FixedPointValue<int, 4>::fromRaw(8)));
    EXPECT_EQ(0.5f, static_cast<float>(meta::FixedPointValue<int, 2>::fromRaw(2)));
    EXPECT_EQ(0.5f, static_cast<float>(meta::FixedPointValue<int, 3>::fromRaw(4)));
    EXPECT_EQ(0.5f, static_cast<float>(meta::FixedPointValue<int, 4>::fromRaw(8)));
}

TEST(FixedPointValueTest, get_integral)
{
    meta::FixedPointValue<int, 16> value(2.25f);
    EXPECT_EQ(2, value.integral());
}

TEST(FixedPointValueTest, unary_negative)
{
    meta::FixedPointValue<int, 2> a(2);
    ASSERT_EQ(2.0f, static_cast<float>(a));
    ASSERT_EQ(-2.0f, static_cast<float>(-a));
}

TEST(FixedPointValueTest, unary_negative_fractional)
{
	meta::FixedPointValue<int, 16> a(2.5f);
	ASSERT_EQ(2.5f, static_cast<float>(a));
	ASSERT_EQ(-2.5f, static_cast<float>(-a));
}