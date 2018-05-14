//
// Created by mzapp on 4/20/18.
//
#pragma once

#include <cstdint>
#include <meta/util/BitSize.h>
#include <meta/util/BitMask.h>

namespace meta
{
//=============================================================================
    template <typename StorageType, std::size_t Bit>
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

    template <typename StorageType, std::size_t PointOffset>
    class FixedPointValue
    {
        static constexpr StorageType Scale = (StorageType)1 << PointOffset;
        using LocalType = FixedPointValue<StorageType, PointOffset>;

    public:
        static constexpr StorageType Radix = PointOffset;

        FixedPointValue() : rawValue(0) {}
        explicit FixedPointValue(int value) : rawValue(value * Scale) {}
        explicit FixedPointValue(std::size_t value) : rawValue(value * Scale) {}
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

        inline LocalType& operator=(float value) { rawValue = value * Scale; return *this; }

        inline LocalType& operator+=(LocalType other) { rawValue += other.rawValue; return *this; }

        inline constexpr static LocalType fromRaw(const StorageType& in)
        {
            LocalType out;
            out.rawValue = in;
            return out;
        }

        inline constexpr StorageType raw() const { return rawValue; }

        inline constexpr StorageType sign() const
        { return meta::signof(rawValue); }

        static inline constexpr LocalType minSigned()
            { return fromRaw(1 << (WordSizeInBits<StorageType>::Value - 1)); }

        static inline constexpr LocalType maxSigned()
            { return fromRaw(BitMask<StorageType, WordSizeInBits<StorageType>::Value - 1>::Right); }

        inline constexpr StorageType integral() const { return rawValue >> PointOffset; }

        inline constexpr StorageType fractional() const
            { return rawValue & FixedPointMaskHelpers<StorageType, PointOffset>::getFractional(); }

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

        inline constexpr float toFloat() const { return static_cast<float>(*this); }
        inline constexpr float toDouble() const { return static_cast<double>(*this); }

        inline constexpr LocalType operator-() const { return fromRaw(rawValue * -1); }

        template <typename ST, std::size_t S>
        using FPV = FixedPointValue<ST,S>;

        // Arithmetic Operators
        template <typename T, std::size_t S> friend constexpr FPV<T,S> operator+ (const FPV<T,S>& a, const FPV<T,S>& b);
        template <typename T, std::size_t S> friend constexpr FPV<T,S> operator- (const FPV<T,S>& a, const FPV<T,S>& b);
        template <typename T, std::size_t S> friend constexpr FPV<T,S> operator* (const FPV<T,S>& a, const FPV<T,S>& b);
        template <typename T, std::size_t S> friend constexpr FPV<T,S> operator/ (const FPV<T,S>& a, const FPV<T,S>& b);

        template <typename T, std::size_t S> friend constexpr FPV<T,S> operator+ (const FPV<T,S>& a, T b);
        template <typename T, std::size_t S> friend constexpr FPV<T,S> operator- (const FPV<T,S>& a, T b);
        template <typename T, std::size_t S> friend constexpr FPV<T,S> operator* (const FPV<T,S>& a, T b);
        template <typename T, std::size_t S> friend constexpr FPV<T,S> operator/ (const FPV<T,S>& a, T b);

        template <typename T, std::size_t S> friend constexpr FPV<T,S> operator+ (T a, const FPV<T,S>& b);
        template <typename T, std::size_t S> friend constexpr FPV<T,S> operator- (T a, const FPV<T,S>& b);
        template <typename T, std::size_t S> friend constexpr FPV<T,S> operator* (T a, const FPV<T,S>& b);
        template <typename T, std::size_t S> friend constexpr FPV<T,S> operator/ (T a, const FPV<T,S>& b);

        template <typename T, std::size_t S> friend constexpr FPV<T,S> operator+ (const FPV<T,S>& a, double b);
        template <typename T, std::size_t S> friend constexpr FPV<T,S> operator- (const FPV<T,S>& a, double b);
        template <typename T, std::size_t S> friend constexpr FPV<T,S> operator* (const FPV<T,S>& a, double b);
        template <typename T, std::size_t S> friend constexpr FPV<T,S> operator/ (const FPV<T,S>& a, double b);

        template <typename T, std::size_t S> friend constexpr FPV<T,S> operator+ (double a, const FPV<T,S>& b);
        template <typename T, std::size_t S> friend constexpr FPV<T,S> operator- (double a, const FPV<T,S>& b);
        template <typename T, std::size_t S> friend constexpr FPV<T,S> operator* (double a, const FPV<T,S>& b);
        template <typename T, std::size_t S> friend constexpr FPV<T,S> operator/ (double a, const FPV<T,S>& b);

        // Logical Operators
        template <typename T, std::size_t S> friend constexpr bool operator <  (const FPV<T,S>& a, const FPV<T,S>& b);
        template <typename T, std::size_t S> friend constexpr bool operator <= (const FPV<T,S>& a, const FPV<T,S>& b);
        template <typename T, std::size_t S> friend constexpr bool operator >  (const FPV<T,S>& a, const FPV<T,S>& b);
        template <typename T, std::size_t S> friend constexpr bool operator >= (const FPV<T,S>& a, const FPV<T,S>& b);

        friend bool operator <  (const LocalType& a, const StorageType& b);
        friend bool operator <= (const LocalType& a, const StorageType& b);
        friend bool operator >  (const LocalType& a, const StorageType& b);
        friend bool operator >= (const LocalType& a, const StorageType& b);

        friend bool operator <  (const LocalType& a, const double& b);
        friend bool operator <= (const LocalType& a, const double& b);
        friend bool operator >  (const LocalType& a, const double& b);
        friend bool operator >= (const LocalType& a, const double& b);

        friend bool operator >= (const LocalType& a, const float& b);
        friend bool operator <  (const LocalType& a, const float& b);
        friend bool operator <= (const LocalType& a, const float& b);
        friend bool operator >  (const LocalType& a, const float& b);

    private:
        StorageType rawValue;
    };
}