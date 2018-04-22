//
// Created by mzapp on 4/10/18.
//

#pragma once
#include <meta/util/fixed_point/Value.h>
#include <meta/util/fixed_point/Math.h>

TEST(FixedPointValueTest, convert_float_to_fixed)
{
    EXPECT_EQ(2, (meta::FixedPointValue<int, 2>(0.5f).raw()));
    EXPECT_EQ(4, (meta::FixedPointValue<int, 3>(0.5f).raw()));
    EXPECT_EQ(8, (meta::FixedPointValue<int, 4>(0.5f).raw()));
}

TEST(FixedPointValueTest, convert_fixed_to_float)
{
    EXPECT_EQ(0.5f, static_cast<float>(meta::FixedPointValue<int, 2>(2)));
    EXPECT_EQ(0.5f, static_cast<float>(meta::FixedPointValue<int, 3>(4)));
    EXPECT_EQ(0.5f, static_cast<float>(meta::FixedPointValue<int, 4>(8)));
    EXPECT_EQ(0.5f, static_cast<float>(meta::FixedPointValue<int, 2>(2)));
    EXPECT_EQ(0.5f, static_cast<float>(meta::FixedPointValue<int, 3>(4)));
    EXPECT_EQ(0.5f, static_cast<float>(meta::FixedPointValue<int, 4>(8)));
}

TEST(FixedPointValueTest, add)
{
    meta::FixedPointValue<int, 3> a(0.5f);
    meta::FixedPointValue<int, 3> b(0.5f);
    meta::FixedPointValue<int, 3> value = a + b;
    EXPECT_EQ(8, value.raw());
    EXPECT_EQ(1.0f, static_cast<float>(value));
}

TEST(FixedPointValueTest, subtract)
{
    meta::FixedPointValue<int, 3> a(0.5f);
    meta::FixedPointValue<int, 3> b(1.0f);
    meta::FixedPointValue<int, 3> value = a - b;
    EXPECT_EQ(-4, value.raw());
    EXPECT_EQ(-0.5f, static_cast<float>(value));
}

TEST(FixedPointValueTest, multiply)
{
    meta::FixedPointValue<int, 3> a(0.5f);
    meta::FixedPointValue<int, 3> b(0.5f);
    meta::FixedPointValue<int, 3> value = a * b;
    EXPECT_EQ(2, value.raw());
    EXPECT_EQ(0.25f, static_cast<float>(value));
}