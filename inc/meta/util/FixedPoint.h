//
// Created by mzapp on 4/20/18.
//
#pragma once

#include <cstddef>

namespace meta
{
//=============================================================================
    template <typename StorageType, size_t PointOffset>
    class FixedPointValue
    {
        typedef FixedPointValue<StorageType, PointOffset> LocalType;
    public:
        FixedPointValue(StorageType value) : m_Value(value) {}

        operator StorageType() const { return m_Value; }

        LocalType& operator=(StorageType value)
        {
            m_Value = value;
            return *this;
        }

        // Copy-type operators
        FixedPointValue(const LocalType& other)
            : m_Value(other.m_Value)
        {}

        FixedPointValue<StorageType, PointOffset>& operator=
            (const LocalType& other)
        {
            m_Value = other.m_Value;
            return *this;
        };

    private:
        StorageType m_Value;
    };

//=============================================================================
    template <typename StorageType, size_t PointOffset>
    class FixedPointHelpers
    {
        using FPType = FixedPointValue<StorageType, PointOffset>;
        static constexpr StorageType Point = (StorageType)1 << PointOffset;

    public:
        static constexpr FixedPointValue<StorageType, PointOffset> floatToFixed(double in)
            { return in * Point; }

        static constexpr float fixedToFloat(const FixedPointValue<StorageType, PointOffset>& in)
        {
            return static_cast<float>(in)
                 / static_cast<float>(Point);
        }

        static constexpr double fixedToFloatHighPrecision
        (const FixedPointValue<StorageType, PointOffset>& in)
        {
            return static_cast<double>(in)
                 / static_cast<double>(Point);
        }
    };

//=============================================================================
    template <typename StorageType, size_t PointOffset>
    static constexpr FixedPointValue<StorageType, PointOffset> operator*
        (const FixedPointValue<StorageType, PointOffset>& a
        , const FixedPointValue<StorageType, PointOffset>& b)
    {
        return FixedPointValue<StorageType, PointOffset>
               (static_cast<long>(a) * static_cast<long>(b) >> PointOffset);
    };

}