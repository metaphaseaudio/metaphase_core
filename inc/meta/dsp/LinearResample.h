//
// Created by Mattb on 6/1/2022.
//

#pragma once

#include <meta/util/math.h>

namespace meta
{
    void linearUpsample(const float* dataIn, float* dataOut, int nSampsIn, int nSampsOut)
    {
        auto resample = float(nSampsOut - 1) / (nSampsIn - 1);

        for (auto s = nSampsOut; --s >= 0;)
        {
            auto in_samp = float(s) / resample;
            auto i = int(std::ceil(in_samp));
            auto j = i == 0 ? 0 : i - 1;

            auto f = i - in_samp;

            dataOut[s] = meta::Interpolate<float>::linear(dataIn[i], dataIn[j], f);
        }
    }
}