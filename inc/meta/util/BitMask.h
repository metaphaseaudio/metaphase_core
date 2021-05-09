//
// Created by mzapp on 5/11/18.
//

#pragma once
#include <meta/util/WordSize.h>

namespace meta
{
    template <typename WordType, std::size_t Bit>
    struct BitMaskHelpers
    {
        inline static constexpr WordType maskBits()
        { return ((WordType)1 << Bit) | BitMaskHelpers<WordType, Bit - 1>::maskBits(); }

        static constexpr WordType maskBitsAbove(unsigned char stopBit)
        { return ((WordType)(Bit > stopBit ? 1 : 0) << Bit)
               | BitMaskHelpers<WordType, Bit - 1>::maskBitsAbove(stopBit); }

    };

    template <typename WordType>
    struct BitMaskHelpers<WordType, 0>
    {
        inline static constexpr WordType maskBits() { return (WordType)1; }

        static constexpr WordType maskBitsAbove(unsigned char stopBit) { return (WordType)0; }
    };

    template <typename WordType, std::size_t BitsToMask>
    struct BitMask
    {
        static constexpr WordType Right = BitMaskHelpers<WordType, BitsToMask - 1>::maskBits();
    };

    template <typename WordType, std::size_t BitsToMask>
    constexpr WordType BitMask<WordType, BitsToMask>::Right;

    template <typename WordType>
    inline static constexpr WordType signof(WordType in)
    {
        return (in & (1 << (WordSizeInBits<WordType>::Value - 1))) < 0 ? - 1 : 1;
    }

    template <size_t Bits>
    struct bitfill { static constexpr size_t value = (static_cast<size_t>(1) << Bits - 1) | bitfill<Bits - 1>::value; };

    template <>
    struct bitfill<1> { static constexpr size_t value = 1; };

    template <>
    struct bitfill<0> { static constexpr size_t value = 0; };
}