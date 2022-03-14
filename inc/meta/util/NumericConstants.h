//
// Created by Matt Zapp on 4/9/2018.
//
//    Various numeric constants. Ok right now it's just float PI.
//

#pragma once
#include <meta/util/BitMask.h>


namespace meta
{
    template <size_t Bits, bool IsSigned=false>
    struct BitInfo
    {
        static constexpr long long Max = IsSigned ? bitfill<Bits - 1>::value : bitfill<Bits>::value;
        static constexpr long long Min = IsSigned ? -(static_cast<long long>(1) << (Bits - 1)) : 0;
    };


    template <typename T>
    struct NumInfo
    {
        static constexpr size_t Bytes = sizeof(T);
        static constexpr size_t Bits = Bytes * 8;
        static constexpr bool IsSigned = std::is_signed<T>::value;
        static constexpr T Max = BitInfo<Bits, IsSigned>::Max;
        static constexpr T Min = BitInfo<Bits, IsSigned>::Min;
    };

    template <typename NumericType>
    struct NumericConstants
    {
        static constexpr NumericType PI = 3.14159265358979323846264338327;
        static constexpr NumericType TWO_PI = 6.28318530717958647692528676656;
        static constexpr NumericType HALF_PI = NumericConstants<NumericType>::PI * 0.5;
    };
}