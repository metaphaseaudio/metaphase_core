//
// Created by Matt Zapp on 4/5/2018.
//
//    The classic 'leaky integrator' which computes a running sum of incoming
//    values, with previous values being weighted less than current values so
//    that old values eventually incur a negligible effect on the output.
//

#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

namespace meta
{
    class OnePoleLowPassFilter
    {
    public:
        OnePoleLowPassFilter()
            : a0(1.0) , b1(0.0), z1(0.0)
        {};

        /// Clear the running sum
        void reset() { z1 = 0; }

        /// Sets the current running sum to a specific value
        void setLastSample(float value) { z1 = value; }

        /**
         * Changes the weights of the integrator based on a frequency and sample rate
         */
        void setCutoff(float sampleRate, float freq)
        {
            b1 = float(exp(-2.0 * M_PI * freq / sampleRate));
            a0 = 1.0f - b1;
        }

        /// Runs a sample through the filter, returns the current running sum.
        float processSample(float in) { return z1 = in * a0 + z1 * b1; }

    protected:
        float a0, b1, z1;
    };
}
