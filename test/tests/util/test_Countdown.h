//
// Created by Matt on 5/3/2022.
//

#pragma once

#include <meta/util/Countdown.h>


TEST(CountdownTest, from_five)
{
    bool hit = false;
    auto countdown = meta::Countdown<5>([&](){ hit = true; });
    ASSERT_EQ(countdown.tick(), 4);
    ASSERT_EQ(countdown.tick(), 3);
    ASSERT_EQ(countdown.tick(), 2);
    ASSERT_EQ(countdown.tick(), 1);
    ASSERT_FALSE(hit);
    ASSERT_EQ(countdown.tick(), 0);
    ASSERT_TRUE(hit);
}


TEST(CountdownTest, from_1)
{
    bool hit = false;
    auto countdown = meta::Countdown<5>([&](){ hit = true; }, 1);
    ASSERT_EQ(countdown.tick(), 0);
    ASSERT_TRUE(hit);
}