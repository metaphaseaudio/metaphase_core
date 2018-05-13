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
    EXPECT_EQ(0.5f, static_cast<float>(meta::FixedPointValue<int, 2>::fromRaw(2)));
    EXPECT_EQ(0.5f, static_cast<float>(meta::FixedPointValue<int, 3>::fromRaw(4)));
    EXPECT_EQ(0.5f, static_cast<float>(meta::FixedPointValue<int, 4>::fromRaw(8)));
    EXPECT_EQ(0.5f, static_cast<float>(meta::FixedPointValue<int, 2>::fromRaw(2)));
    EXPECT_EQ(0.5f, static_cast<float>(meta::FixedPointValue<int, 3>::fromRaw(4)));
    EXPECT_EQ(0.5f, static_cast<float>(meta::FixedPointValue<int, 4>::fromRaw(8)));
}

TEST(FixedPointValueTest, unary_negative)
{
    meta::FixedPointValue<int, 2> a(2);
    ASSERT_EQ(2.0f, static_cast<float>(a));
    ASSERT_EQ(-2.0f, static_cast<float>(-a));
}


//TEST(FixedPointValueTest, add)
//{
//    meta::FixedPointValue<int, 3> a(0.5f);
//    meta::FixedPointValue<int, 3> b(0.5f);
//    EXPECT_EQ(8, (a + b).raw());
//    EXPECT_EQ(1.0f, static_cast<float>(a + b));
//    EXPECT_EQ(1.5f, static_cast<float>(a + 1));
//    EXPECT_EQ(1.5f, static_cast<float>(1 + a));
//    EXPECT_EQ(1.5f, static_cast<float>(a + 1.0f));
//    EXPECT_EQ(1.5f, static_cast<float>(1.0f + a));
//    EXPECT_EQ(1.5f, static_cast<float>(a + 1.0));
//    EXPECT_EQ(1.5f, static_cast<float>(1.0 + a));
//}
//
//TEST(FixedPointValueTest, subtract)
//{
//    meta::FixedPointValue<int, 3> a(0.5f);
//    meta::FixedPointValue<int, 3> b(1.0f);
//    EXPECT_EQ(-4, (a - b).raw());
//    EXPECT_EQ(-0.5f, static_cast<float>(a - b));
//    EXPECT_EQ(-0.5f, static_cast<float>(a - 1));
//    EXPECT_EQ(0.5f, static_cast<float>(1 - a));
//    EXPECT_EQ(-0.5f, static_cast<float>(a - 1.0f));
//    EXPECT_EQ(0.5f, static_cast<float>(1.0f - a));
//    EXPECT_EQ(-0.5f, static_cast<float>(a - 1.0));
//    EXPECT_EQ(0.5f, static_cast<float>(1.0 - a));
//}
//
//TEST(FixedPointValueTest, multiply)
//{
//    meta::FixedPointValue<int, 3> a(0.5f);
//    meta::FixedPointValue<int, 3> b(0.5f);
//
//    EXPECT_EQ(2, (a * b).raw());
//    EXPECT_EQ(0.25f, static_cast<float>(a * b));
//    EXPECT_EQ(1.0f, static_cast<float>(2 * a));
//    EXPECT_EQ(1.0f, static_cast<float>(a * 2));
//}
//
//TEST(FixedPointValueTest, abs)
//{
//    meta::FixedPointValue<int, 3> a(0.5f);
//    meta::FixedPointValue<int, 3> b(0.5f);
//
//    EXPECT_EQ(2, (a * b).raw());
//    EXPECT_EQ(0.25f, static_cast<float>(a * b));
//    EXPECT_EQ(1.0f, static_cast<float>(2 * a));
//    EXPECT_EQ(1.0f, static_cast<float>(a * 2));
//}