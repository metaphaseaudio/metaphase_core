//
// Created by mzapp on 4/20/18.
//
#pragma once

namespace meta
{
//=============================================================================
    template <typename StorageType, size_t Bit>
    struct FixedPointMaskHelpers
    {
        inline static constexpr StorageType getFractional()
        {
            return ((StorageType)1 << Bit) + FixedPointMaskHelpers<StorageType, Bit - 1>::getFractional();
        }
    };

    template <typename StorageType>
    struct FixedPointMaskHelpers<StorageType, 0>
    {
        inline static constexpr StorageType getFractional() { return 1; }
    };

    template <typename StorageType, unsigned char PointOffset>
    class FixedPointValue
    {
        static constexpr StorageType Scale = (StorageType)1 << PointOffset;
        using LocalType = FixedPointValue<StorageType, PointOffset>;

    public:
        static constexpr StorageType Radix = PointOffset;

        FixedPointValue() : rawValue(0) {}
        explicit FixedPointValue(StorageType value) : rawValue(value) {}

        explicit FixedPointValue(int value) : rawValue(value * Scale) {}
        explicit FixedPointValue(long value) : rawValue(value * Scale) {}
        explicit FixedPointValue(size_t value) : rawValue(value * Scale) {}
        explicit FixedPointValue(float value) : rawValue(value * Scale) {}
        explicit FixedPointValue(double value) : rawValue(value * Scale) {}

        FixedPointValue(const LocalType& other) : rawValue(other.rawValue) {}

        inline FixedPointValue<StorageType, PointOffset>& operator=(const LocalType& other)
        {
            rawValue = other.rawValue;
            return *this;
        };

        inline LocalType& operator=(StorageType value)
        {
            rawValue = value;
            return *this;
        }

        inline LocalType& operator=(float value)
        {
            rawValue = value * Scale;
            return *this;
        }

        inline LocalType& operator+=(LocalType other)
        {
            rawValue += other.rawValue;
            return *this;
        }

        inline constexpr static LocalType fromRaw(const StorageType& in)
        {
            LocalType out;
            out.rawValue = in;
            return out;
        }

        inline constexpr StorageType raw() const { return rawValue; }

        inline constexpr StorageType integralPart() const { return rawValue >> PointOffset; }

        inline constexpr StorageType fractionalPart() const
        {
            return rawValue & FixedPointMaskHelpers<StorageType, PointOffset - 1>::getFractional();
        }

        inline constexpr explicit operator float() const
        {
            return static_cast<float>(rawValue)
                 / static_cast<float>(Scale);
        }

        inline constexpr explicit operator double() const
        {
            return static_cast<double>(rawValue)
                 / static_cast<double>(Scale);
        }

        inline constexpr bool operator < (const LocalType& other) { return rawValue < other.rawValue; }
        inline constexpr bool operator <= (const LocalType& b) { return rawValue <= b.rawValue; }

        inline constexpr bool operator > (const LocalType& other) { return rawValue > other.rawValue; }
        inline constexpr bool operator >= (const LocalType& b) { return rawValue >= b.rawValue; }

    private:
        StorageType rawValue;
    };
}