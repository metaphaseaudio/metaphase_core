//
// Created by mzapp on 4/20/18.
//
#pragma once

namespace meta
{
//=============================================================================
    template <typename StorageType, unsigned char PointOffset>
    class FixedPointValue
    {
        static constexpr StorageType Scale = (StorageType)1 << PointOffset;
        using LocalType = FixedPointValue<StorageType, PointOffset>;

    public:
        explicit FixedPointValue(StorageType value) : m_Value(value) {}
        explicit FixedPointValue(float value) : m_Value(value * Scale) {}
        explicit FixedPointValue(double value) : m_Value(value * Scale) {}
        FixedPointValue(const LocalType& other) : m_Value(other.m_Value) {}

        FixedPointValue<StorageType, PointOffset>& operator=
            (const LocalType& other)
        {
            m_Value = other.m_Value;
            return *this;
        };

        LocalType& operator=(StorageType value)
        {
            m_Value = value;
            return *this;
        }

        StorageType raw() const { return m_Value; }

        explicit operator float() const
        {
            return static_cast<float>(m_Value)
                 / static_cast<float>(Scale);
        }

        explicit operator double() const
        {
            return static_cast<double>(m_Value)
                 / static_cast<double>(Scale);
        }

    private:
        StorageType m_Value;
    };
}