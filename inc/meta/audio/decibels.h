//
// Created by Matt on 11/14/2021.
//

#pragma once

namespace meta
{
    template <typename NumType>
    NumType gain_coeff_to_db(NumType x)
    {
        if (x == NumType(0)) { return -std::numeric_limits<NumType>::infinity(); }
        return NumType(20) * std::log10(x);
    }
}