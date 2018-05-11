//
// Created by mzapp on 4/19/18.
//
#pragma once

#include <climits>   // CHAR_BIT
#include <cstddef>   // std::size_t

namespace meta
{
    template <typename T>
    struct WordSizeInBits
    {
        static constexpr std::size_t Value() { return sizeof(T) * CHAR_BIT; };
    };
}
