//
// Created by Matt on 2/15/2021.
//

#pragma once
#include <meta/util/math.h>
#include <meta/util/container_helpers/array.h>
#include <type_traits>

namespace meta
{
    template <typename T, T start, T step, size_t size>
    struct sequence{
        static constexpr std::array<T, size> value =
                concat_array(std::array<T, 1>{start}, sequence<T, start + step, step, size - 1>::value);
    };

    template <typename T, T start, T step>
    struct sequence<T, start, step, 1> {
        static constexpr std::array<T, 1> value = { start };
    };

    template <typename T, T start, T end, T step>
    struct range_sequence {
        static constexpr size_t size = static_cast<size_t>((end - start) / step);
        static constexpr std::array<T, size> value = sequence<T, start, step, size>::value;
    };
}
