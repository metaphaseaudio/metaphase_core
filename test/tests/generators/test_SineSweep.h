//
// Created by Matt on 4/26/2020.
//
#pragma once

#include <meta/generators/SineSweep.h>


TEST(SineSweepTest, one_tick)
{
    auto sweep = meta::SineSweep<float, 10>(1, 5, 20);
    EXPECT_EQ(sweep.tick(), 0);
    EXPECT_EQ(sweep.tick(), 0.5);
}
