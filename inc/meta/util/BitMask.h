//
// Created by mzapp on 5/11/18.
//

#pragma once
#include <meta/util/BitSize.h>

namespace meta
{
    template <typename WordType, std::size_t Bit>
    struct BitMaskHelpers
    {
        inline static constexpr WordType maskBits()
        { return ((WordType)1 << Bit) | BitMaskHelpers<WordType, Bit - 1>::maskBits(); }
    };

    template <typename WordType>
    struct BitMaskHelpers<WordType, 0>
    {
        inline static constexpr WordType maskBits() { return (WordType)1; }
    };

    template <typename WordType>
    struct BitMask
    {
        template <std::size_t BitsToMask>
        inline static constexpr WordType Right()
        {
            return BitMaskHelpers<WordType, BitsToMask - 1>::maskBits();
        }

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshift-overflow"
        template <std::size_t BitsToMask>
        inline static constexpr WordType Left()
        {
            return BitMaskHelpers<WordType, BitsToMask>::maskBits()
                << (WordSizeInBits<WordType>::Value() - BitsToMask);
        }
#pragma clang diagnostic pop

        inline static constexpr WordType Sign(WordType in)
            { return in & BitMask<WordType>::Left<1>() & 1; }
    };
}