//
// Created by mzapp on 4/22/18.
//

#pragma once

#include "Value.h"

namespace meta
{
    template <typename StorageType, unsigned char PointOffset>
    static constexpr FixedPointValue<StorageType, PointOffset> operator+
    (const FixedPointValue<StorageType, PointOffset>& a, const FixedPointValue<StorageType, PointOffset>& b)
        { return FixedPointValue<StorageType, PointOffset>(static_cast<StorageType>(a.raw()+ b.raw())); };

    template <typename StorageType, unsigned char PointOffset>
    static constexpr FixedPointValue<StorageType, PointOffset> operator-
    (const FixedPointValue<StorageType, PointOffset>& a, const FixedPointValue<StorageType, PointOffset>& b)
    {
        return FixedPointValue<StorageType, PointOffset>(static_cast<StorageType>(a.raw()- b.raw()));
    };

    template <typename StorageType, unsigned char PointOffset>
    static constexpr FixedPointValue<StorageType, PointOffset> operator*
    (const FixedPointValue<StorageType, PointOffset>& a, const FixedPointValue<StorageType, PointOffset>& b)
    {
        const long araw = a.raw();
        const long braw = b.raw();
        const long value = araw * braw >> PointOffset;
        return FixedPointValue<StorageType, PointOffset>(static_cast<StorageType>(value));
    };

    template <typename StorageType, unsigned char PointOffset>
    static constexpr FixedPointValue<StorageType, PointOffset> operator*
    (const FixedPointValue<StorageType, PointOffset>& a, float b)
    {
        const long araw = a.raw();
        const long braw = FixedPointValue<StorageType, PointOffset>(b).raw();
        const long value = araw * braw >> PointOffset;
        return FixedPointValue<StorageType, PointOffset>(static_cast<StorageType>(value));
    };
}
