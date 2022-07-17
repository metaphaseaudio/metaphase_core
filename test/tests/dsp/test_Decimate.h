//
// Created by Mattb on 7/16/2022.
//

#pragma once

#include <meta/dsp/Decimate.h>


TEST(DecimateTest, nth)
{
    float data[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    float out[4] = {0};
    meta::Decimate<float, 16, 4> decimate;
    decimate(data, out, 16);
    ASSERT_EQ(out[0], 0);
    ASSERT_EQ(out[1], 4);
    ASSERT_EQ(out[2], 8);
    ASSERT_EQ(out[3], 12);
}
