//
// Created by Mattb on 6/1/2022.
//

#pragma once

#include <meta/dsp/LinearResample.h>


TEST(LinearResampleTest, linear_upsample)
{
    constexpr int sizeIn = 3;
    constexpr int sizeOut = 5;
    float in[sizeIn] = {0, 4};
    float out[sizeOut] = {0};

    meta::linearUpsample(in, out, sizeIn, sizeOut);
    for (int i = 0; i < 5; i++)
    {
        ASSERT_EQ(out[i], i);
    }
}