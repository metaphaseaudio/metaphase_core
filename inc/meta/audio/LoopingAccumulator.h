//
// Created by Matt on 5/18/2021.
//

#pragma once
#include <meta/util/math.h>


namespace meta
{

    class LoopingAccumulator
    {
    public:
        LoopingAccumulator(float min, float max, float sample_rate, float frequency=20.0f)
            : min(min)
            , max(max)
            , range(meta::abs(max - min))
            , sample_rate(sample_rate)
        { set_freq(frequency); }

        void sync(float new_value)
            { value = meta::limit(min + 1, max, new_value); }

        void set_freq(float freq)
        {
            // At least two samples are required to represent nyquist. Higher than this shall not pass.
            const auto samples_per_cycle = std::max(2.0f, sample_rate / freq);
            const float frange = range;
            delta = frange / samples_per_cycle;
        }

        void tick()
        {
            value += delta;
            if (std::floor(value) > max)
                { value -= range + 1; }
        }

        [[ nodiscard ]] float getValue() const { return value; }

    private:
        const float min, max, range;
        float sample_rate = 48000;
        float delta = 0.0f;
        float value = 0.0f;
    };
}