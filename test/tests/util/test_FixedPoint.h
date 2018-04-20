//
// Created by mzapp on 4/10/18.
//

#pragma once
#include <meta/util/FixedPoint.h>

TEST(FixedPointHelpersTest, convert_float_to_fixed)
{
    EXPECT_EQ(2, (meta::FixedPointHelpers<int, 2>::floatToFixed(0.5f)));
    EXPECT_EQ(4, (meta::FixedPointHelpers<int, 3>::floatToFixed(0.5f)));
    EXPECT_EQ(8, (meta::FixedPointHelpers<int, 4>::floatToFixed(0.5f)));
}

TEST(FixedPointHelpersTest, convert_fixed_to_float)
{
    EXPECT_EQ(0.5, (meta::FixedPointHelpers<int, 2>::fixedToFloat(2)));
    EXPECT_EQ(0.5, (meta::FixedPointHelpers<int, 3>::fixedToFloat(4)));
    EXPECT_EQ(0.5, (meta::FixedPointHelpers<int, 4>::fixedToFloat(8)));
    EXPECT_EQ(0.5, (meta::FixedPointHelpers<int, 2>::fixedToFloatHighPrecision(2)));
    EXPECT_EQ(0.5, (meta::FixedPointHelpers<int, 3>::fixedToFloatHighPrecision(4)));
    EXPECT_EQ(0.5, (meta::FixedPointHelpers<int, 4>::fixedToFloatHighPrecision(8)));
}

TEST(FixedPointHelpersTest, multiply)
{
    meta::FixedPointValue<int, 3> a(4);
    meta::FixedPointValue<int, 3> b(4);
    int value = a * b;
    EXPECT_EQ(2, value);
    EXPECT_EQ(0.25, (meta::FixedPointHelpers<int, 3>::fixedToFloat(value)));
}