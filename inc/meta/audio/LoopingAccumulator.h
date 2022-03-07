//
// Created by Matt on 5/18/2021.
//

#pragma once
#include <meta/util/math.h>


namespace meta
{
    template <int Min, unsigned int Max>
    struct LoopingAccumulator
    {
        static constexpr unsigned long Range = meta::abs(Max - Min);

        size_t sample_rate = 48000;
        float delta = 0.0f;
        float value = 0.0f;

        void sync(float new_value) { value = std::max<float>(std::min<float>(new_value, Max), Min + 1); }

        void set_freq(float freq)
        {
            // At least two samples are required to represent nyquist. Higher than this shall not pass.
            const auto samples_per_cycle = std::max(2.0f, sample_rate / freq);
            const float frange = Range;
            delta = frange / samples_per_cycle;
        }

        void tick()
        {
            value += delta;
            if (std::floor(value) > Max) { value -= Range; }
        }
    };
}