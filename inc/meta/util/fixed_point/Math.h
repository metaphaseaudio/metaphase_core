//
// Created by mzapp on 4/22/18.
//

#pragma once

#include "Value.h"

namespace meta
{
    // Logical  -- Same Type ==================================================
    template<typename StorageType, std::size_t PointOffset>
    inline constexpr bool operator<
            (const FixedPointValue <StorageType, PointOffset> &a,
             const FixedPointValue <StorageType, PointOffset> &b)
    { return a.rawValue < b.rawValue; }

    template<typename StorageType, std::size_t PointOffset>
    inline constexpr bool operator<=
            (const FixedPointValue <StorageType, PointOffset> &a,
             const FixedPointValue <StorageType, PointOffset> &b)
    { return a.rawValue <= b.rawValue; }

    template<typename StorageType, std::size_t PointOffset>
    inline constexpr bool operator>
            (const FixedPointValue <StorageType, PointOffset> &a,
             const FixedPointValue <StorageType, PointOffset> &b)
    { return a.rawValue > b.rawValue; }

    template<typename StorageType, std::size_t PointOffset>
    inline constexpr bool operator>=
            (const FixedPointValue <StorageType, PointOffset> &a,
             const FixedPointValue <StorageType, PointOffset> &b)
    { return a.rawValue >= b.rawValue; }

    // Logical  -- Storage Type ================================================
    template<typename StorageType, std::size_t PointOffset>
    inline constexpr bool operator<
            (const FixedPointValue <StorageType, PointOffset> &a, StorageType b)
    { return a.rawValue < FixedPointValue<StorageType, PointOffset>(b).rawValue; }

    template<typename StorageType, std::size_t PointOffset>
    inline constexpr bool operator<=
            (const FixedPointValue <StorageType, PointOffset> &a, StorageType b)
    { return a.rawValue <= FixedPointValue<StorageType, PointOffset>(b).rawValue; }

    template<typename StorageType, std::size_t PointOffset>
    inline constexpr bool operator>
            (const FixedPointValue <StorageType, PointOffset> &a, StorageType b)
    { return a.rawValue > FixedPointValue<StorageType, PointOffset>(b).rawValue; }

    template<typename StorageType, std::size_t PointOffset>
    inline constexpr bool operator>=
            (const FixedPointValue <StorageType, PointOffset> &a, StorageType b)
    { return a.rawValue >= FixedPointValue<StorageType, PointOffset>(b).rawValue; }

    // Inverse
    template<typename StorageType, std::size_t PointOffset>
    inline constexpr bool operator<
            (StorageType a, const FixedPointValue <StorageType, PointOffset> &b)
    { return FixedPointValue<StorageType, PointOffset>(a).rawValue < b.rawValue; }

    template<typename StorageType, std::size_t PointOffset>
    inline constexpr bool operator<=
            (StorageType a, const FixedPointValue <StorageType, PointOffset> &b)
    { return FixedPointValue<StorageType, PointOffset>(a).rawValue <= b.rawValue; }

    template<typename StorageType, std::size_t PointOffset>
    inline constexpr bool operator>
            (StorageType a, const FixedPointValue <StorageType, PointOffset> &b)
    { return FixedPointValue<StorageType, PointOffset>(a).rawValue > b.rawValue; }

    template<typename StorageType, std::size_t PointOffset>
    inline constexpr bool operator>=
            (StorageType a, const FixedPointValue <StorageType, PointOffset> &b)
    { return FixedPointValue<StorageType, PointOffset>(a).rawValue >= b.rawValue; }


    // Logical -- Double Type =================================================
    template<typename StorageType, std::size_t PointOffset>
    inline constexpr bool operator<
            (const FixedPointValue <StorageType, PointOffset> &a, double b)
    { return a.rawValue < FixedPointValue<StorageType, PointOffset>(b).rawValue; }

    template<typename StorageType, std::size_t PointOffset>
    inline constexpr bool operator<=
            (const FixedPointValue <StorageType, PointOffset> &a, double b)
    { return a.rawValue <= FixedPointValue<StorageType, PointOffset>(b).rawValue; }

    template<typename StorageType, std::size_t PointOffset>
    inline constexpr bool operator>
            (const FixedPointValue <StorageType, PointOffset> &a, double b)
    { return a.rawValue > FixedPointValue<StorageType, PointOffset>(b).rawValue; }

    template<typename StorageType, std::size_t PointOffset>
    inline constexpr bool operator>=
            (const FixedPointValue <StorageType, PointOffset> &a, double b)
    { return a.rawValue >= FixedPointValue<StorageType, PointOffset>(b).rawValue; }

    // Logical  -- Float Type ================================================
    template<typename StorageType, std::size_t PointOffset>
    inline constexpr bool operator<
            (const FixedPointValue <StorageType, PointOffset> &a, float b)
    { return a.rawValue < FixedPointValue<StorageType, PointOffset>(b).rawValue; }

    template<typename StorageType, std::size_t PointOffset>
    inline constexpr bool operator<=
            (const FixedPointValue <StorageType, PointOffset> &a, float b)
    { return a.rawValue <= FixedPointValue<StorageType, PointOffset>(b).rawValue; }

    template<typename StorageType, std::size_t PointOffset>
    inline constexpr bool operator>
            (const FixedPointValue <StorageType, PointOffset> &a, float b)
    { return a.rawValue > FixedPointValue<StorageType, PointOffset>(b).rawValue; }

    template<typename StorageType, std::size_t PointOffset>
    inline constexpr bool operator>=
            (const FixedPointValue <StorageType, PointOffset> &a, float b)
    { return a.rawValue >= FixedPointValue<StorageType, PointOffset>(b).rawValue; }


    // Arithmetic -- Same Type ================================================
    template<typename StorageType, std::size_t PointOffset>
    inline constexpr FixedPointValue <StorageType, PointOffset> operator+
            (const FixedPointValue <StorageType, PointOffset> &a,
             const FixedPointValue <StorageType, PointOffset> &b)
    {
        const auto out = static_cast<StorageType>(a.rawValue + b.rawValue);
        return FixedPointValue<StorageType, PointOffset>::fromRaw(out);
    };

    template<typename StorageType, std::size_t PointOffset>
    inline constexpr FixedPointValue <StorageType, PointOffset> operator-
            (const FixedPointValue <StorageType, PointOffset> &a,
             const FixedPointValue <StorageType, PointOffset> &b)
    {
        const auto out = static_cast<StorageType>(a.rawValue - b.rawValue);
        return FixedPointValue<StorageType, PointOffset>::fromRaw(out);
    };

    template<typename StorageType, std::size_t PointOffset>
    inline constexpr FixedPointValue <StorageType, PointOffset> operator*
            (const FixedPointValue <StorageType, PointOffset> &a,
             const FixedPointValue <StorageType, PointOffset> &b)
    {
        const long value = long(a.rawValue) * b.rawValue >> PointOffset;
        return FixedPointValue<StorageType, PointOffset>::fromRaw(static_cast<StorageType>(value));
    };

    template<typename StorageType, std::size_t PointOffset>
    inline constexpr FixedPointValue <StorageType, PointOffset> operator/
            (const FixedPointValue <StorageType, PointOffset> &a,
             const FixedPointValue <StorageType, PointOffset> &b)
    {
        const long value = (long(a.rawValue) << PointOffset) / b.rawValue;
        return FixedPointValue<StorageType, PointOffset>::fromRaw(static_cast<StorageType>(value));
    };


    // Arithmetic -- Storage Type ================================================
    template<typename StorageType, std::size_t PointOffset>
    inline constexpr FixedPointValue <StorageType, PointOffset> operator+
            (const FixedPointValue <StorageType, PointOffset> &a, StorageType b)
    {
        const auto bFixed = FixedPointValue<StorageType, PointOffset>(b);
        const auto out = static_cast<StorageType>(a.rawValue + bFixed.rawValue);
        return FixedPointValue<StorageType, PointOffset>::fromRaw(out);
    };

    template<typename StorageType, std::size_t PointOffset>
    inline constexpr FixedPointValue <StorageType, PointOffset> operator-
            (const FixedPointValue <StorageType, PointOffset> &a, StorageType b)
    {
        const auto bFixed = FixedPointValue<StorageType, PointOffset>(b);
        const auto out = static_cast<StorageType>(a.rawValue - bFixed.rawValue);
        return FixedPointValue<StorageType, PointOffset>::fromRaw(out);
    };

    template<typename StorageType, std::size_t PointOffset>
    inline constexpr FixedPointValue <StorageType, PointOffset> operator*
            (const FixedPointValue <StorageType, PointOffset> &a, StorageType b)
    {
        const auto bFixed = FixedPointValue<StorageType, PointOffset>(b);
        const long value = long(a.rawValue) * bFixed.rawValue >> PointOffset;
        return FixedPointValue<StorageType, PointOffset>::fromRaw(static_cast<StorageType>(value));
    };

    template<typename StorageType, std::size_t PointOffset>
    inline constexpr FixedPointValue <StorageType, PointOffset> operator/
            (const FixedPointValue <StorageType, PointOffset> &a, StorageType b)
    {
        const auto bFixed = FixedPointValue<StorageType, PointOffset>(b);
        const long value = (a.rawValue << PointOffset) / bFixed.rawValue;
        return FixedPointValue<StorageType, PointOffset>::fromRaw(static_cast<StorageType>(value));
    };

    // Inverse
    template<typename StorageType, std::size_t PointOffset>
    inline constexpr FixedPointValue <StorageType, PointOffset> operator+
            (StorageType a, const FixedPointValue <StorageType, PointOffset> &b)
    {
        const auto aFixed = FixedPointValue<StorageType, PointOffset>(a);
        const auto out = static_cast<StorageType>(aFixed.rawValue + b.rawValue);
        return FixedPointValue<StorageType, PointOffset>::fromRaw(out);
    };

    template<typename StorageType, std::size_t PointOffset>
    constexpr FixedPointValue <StorageType, PointOffset> operator-
            (StorageType a, const FixedPointValue <StorageType, PointOffset> &b)
    {
        const auto aFixed = FixedPointValue<StorageType, PointOffset>(a);
        const auto out = static_cast<StorageType>(aFixed.rawValue - b.rawValue);
        return FixedPointValue<StorageType, PointOffset>::fromRaw(out);
    };

    template<typename StorageType, std::size_t PointOffset>
    inline constexpr FixedPointValue <StorageType, PointOffset> operator*
            (StorageType a, const FixedPointValue <StorageType, PointOffset> &b)
    {
        const auto aFixed = FixedPointValue<StorageType, PointOffset>(a);
        const long value = long(aFixed.rawValue) * b.rawValue >> PointOffset;
        return FixedPointValue<StorageType, PointOffset>::fromRaw(static_cast<StorageType>(value));
    };

    template<typename StorageType, std::size_t PointOffset>
    inline constexpr FixedPointValue <StorageType, PointOffset> operator/
            (StorageType a, const FixedPointValue <StorageType, PointOffset> &b)
    {
        const auto aFixed = FixedPointValue<StorageType, PointOffset>(a);
        const long value = (aFixed.rawValue << PointOffset) / b.rawValue;
        return FixedPointValue<StorageType, PointOffset>::fromRaw(static_cast<StorageType>(value));
    };

    // Arithmetic -- Double Type ==============================================
    template<typename StorageType, std::size_t PointOffset>
    inline constexpr FixedPointValue <StorageType, PointOffset> operator+
            (const FixedPointValue <StorageType, PointOffset> &a, double b)
    {
        const auto bFixed = FixedPointValue<StorageType, PointOffset>(b);
        const auto out = static_cast<StorageType>(a.rawValue + bFixed.rawValue);
        return FixedPointValue<StorageType, PointOffset>::fromRaw(out);
    };

    template<typename StorageType, std::size_t PointOffset>
    inline constexpr FixedPointValue <StorageType, PointOffset> operator-
            (const FixedPointValue <StorageType, PointOffset> &a, double b)
    {
        const auto bFixed = FixedPointValue<StorageType, PointOffset>(b);
        const auto out = static_cast<StorageType>(a.rawValue - bFixed.rawValue);
        return FixedPointValue<StorageType, PointOffset>::fromRaw(out);
    };

    template<typename StorageType, std::size_t PointOffset>
    inline constexpr FixedPointValue <StorageType, PointOffset> operator*
            (const FixedPointValue <StorageType, PointOffset> &a, double b)
    {
        const auto bFixed = FixedPointValue<StorageType, PointOffset>(b);
        const long value = long(a.rawValue) * bFixed.rawValue >> PointOffset;
        return FixedPointValue<StorageType, PointOffset>::fromRaw(static_cast<StorageType>(value));
    };

    template<typename StorageType, std::size_t PointOffset>
    inline constexpr FixedPointValue <StorageType, PointOffset> operator/
            (const FixedPointValue <StorageType, PointOffset> &a, double b)
    {
        const auto bFixed = FixedPointValue<StorageType, PointOffset>(b);
        const long value = (a.rawValue << PointOffset) / bFixed.rawValue;
        return FixedPointValue<StorageType, PointOffset>::fromRaw(static_cast<StorageType>(value));
    };

    // Inverse
    template<typename StorageType, std::size_t PointOffset>
    inline constexpr FixedPointValue <StorageType, PointOffset> operator+
    (double a, const FixedPointValue <StorageType, PointOffset>& b)
    {
        const auto aFixed = FixedPointValue<StorageType, PointOffset>(a);
        const auto out = static_cast<StorageType>(aFixed.rawValue + b.rawValue);
        return FixedPointValue<StorageType, PointOffset>::fromRaw(out);
    };

    template<typename StorageType, std::size_t PointOffset>
    inline constexpr FixedPointValue <StorageType, PointOffset> operator-
    (double a, const FixedPointValue <StorageType, PointOffset>& b)
    {
        const auto aFixed = FixedPointValue<StorageType, PointOffset>(a);
        const auto out = static_cast<StorageType>(aFixed.rawValue - b.rawValue);
        return FixedPointValue<StorageType, PointOffset>::fromRaw(out);
    };

    template<typename StorageType, std::size_t PointOffset>
    inline constexpr FixedPointValue <StorageType, PointOffset> operator*
    (double a, const FixedPointValue <StorageType, PointOffset>& b)
    {
        const auto aFixed = FixedPointValue<StorageType, PointOffset>(a);
        const long value = long(aFixed.rawValue) * b.rawValue >> PointOffset;
        return FixedPointValue<StorageType, PointOffset>::fromRaw(static_cast<StorageType>(value));
    };

    template<typename StorageType, std::size_t PointOffset>
    inline constexpr FixedPointValue <StorageType, PointOffset> operator/
    (double a, const FixedPointValue <StorageType, PointOffset>& b)
    {
        const auto aFixed = FixedPointValue<StorageType, PointOffset>(a);
        const long value = (aFixed.rawValue << PointOffset) / b.rawValue;
        return FixedPointValue<StorageType, PointOffset>::fromRaw(static_cast<StorageType>(value));
    };

    //=========================================================================
    template<typename StorageType, std::size_t PointOffset>
    constexpr FixedPointValue <StorageType, PointOffset> abs
    (const FixedPointValue <StorageType, PointOffset>& in)
    {
        const auto sign = in.sign();
        return sign > 0 ? in : -in;
    };
}
