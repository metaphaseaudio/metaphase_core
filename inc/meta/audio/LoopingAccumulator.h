//
// Created by Matt on 5/18/2021.
//

#pragma once
#include <meta/util/math.h>
#include <functional>


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
            , end(min)
        {
//            jassert(min != max);
            set_freq(frequency);
            sync(min);
        }

        void set_end(float end_value) { end = end_value; }


        void sync(float new_value)
            { value = meta::limit(min, max, new_value); }

        void set_freq(float inFreq)
        {
            // At least two samples are required to represent nyquist. Higher than this shall not pass.
            freq = inFreq;
            const auto samples_per_cycle = std::max(2.0f, sample_rate / inFreq);
            delta = range / samples_per_cycle;
        }

        void set_sample_rate(float sr)
        {
            sample_rate = sr;
            set_freq(freq);
        }

        float tick()
        {
            const auto rv = value;

            value += delta;

            if (end > min && value >= end)
                { value = end; }

            if (value > max)
            {
                value -= range;
                onReset(value);
            }

            return rv;
        }

        [[ nodiscard ]] float getValue() const { return value; }

        const float min, max, range;

        std::function<void(float)> onReset = [](float){};

    private:
        float freq = 250;
        float sample_rate = 48000;
        float delta = 0.0f;
        float value = 0.0f;
        float end = max;
    };
}