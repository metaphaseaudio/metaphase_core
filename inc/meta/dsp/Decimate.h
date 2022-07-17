//
// Created by Mattb on 7/16/2022.
//

#pragma once

#include <meta/util/math.h>

namespace meta
{
    template <typename NumType, size_t inSize, size_t outSize>
    class Decimate
    {
    public:
        static constexpr float ratio = float(inSize) / float(outSize);

        void operator()(const NumType* in, NumType* decimated, size_t nSampsIn)
        {
            float nSamps = nSampsIn / ratio;
            for (int outSamp = 0; outSamp < nSamps; outSamp++)
            {
                float phase = outSamp * ratio;
                auto i = static_cast<int>(phase);
                float f = phase - i;
                int j = i + 1;
                decimated[outSamp] = meta::Interpolate<float>::linear(in[i], j < nSampsIn ? in[j] : in[i], f);
            }
        }
    };
}