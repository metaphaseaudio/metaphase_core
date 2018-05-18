//
// Created by Matt Zapp on 4/28/2018.
//

#pragma once

#include <cmath>

namespace meta
{
    template <typename NumericType>
    NumericType wrap(NumericType x, NumericType lower, NumericType upper)
    {
        auto range = upper - lower + 1;

        if (x < lower)
            { x += range * ((lower - x) / range + 1); }

        return lower + fmodf(x - lower, range);
    }

    template <typename NumericType>
    constexpr NumericType limit(NumericType low, NumericType high, NumericType value)
    { return value < low ? low : (high < value ? high : value); }

	 template <typename T> T abs(T i) { return std::abs(i); }
	template <typename T> T fabs(T i) { return std::fabs(i); }

    template <typename NumericType>
    struct Interpolate
    {
        static constexpr NumericType linear(NumericType a, NumericType b, NumericType weight)
        { return (a * weight) + (b * (1 - weight)); }

        static constexpr NumericType parabolic
        (NumericType a, NumericType b, NumericType weight, NumericType exponent=2)
        {
            if (fabs(exponent) < 0.001) { return linear(a, b, weight); }
            auto ratio = (1 - exp(exponent * weight)) / (1 - exp(exponent));
            return a + (b - a) * ratio;
        }
    };
}
