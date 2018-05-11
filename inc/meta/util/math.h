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
}
