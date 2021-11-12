//
// Created by Matt on 11/10/2021.
//

#pragma once

#include <meta/util/math.h>


TEST(MetaMathInterpolate, linear)
{
    EXPECT_EQ(meta::Interpolate<float>::linear(0, 100, 0), 0);
    EXPECT_EQ(meta::Interpolate<float>::linear(0, 100, 1), 100);
    EXPECT_EQ(meta::Interpolate<float>::linear(0, 100, 0.5), 50);
}
