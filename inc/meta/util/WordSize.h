//
// Created by mzapp on 4/19/18.
//
#pragma once

#include <cstddef>
#include <climits>
#include <cstdint>

namespace meta
{
    template <typename T>
    struct WordSizeInBits { static constexpr std::size_t Value = sizeof(T) * CHAR_BIT; };

    template <typename T>
    constexpr std::size_t WordSizeInBits<T>::Value;

	// Expand word
    template <typename T>
    struct Expand {};

	template <>
	struct Expand<int8_t>
	{
		using Type = int16_t;
		inline static constexpr Type cast(int8_t in) { return static_cast<Type>(in); }
	};

	template <>
	struct Expand<int16_t>
	{
		using Type = int32_t;
		inline static constexpr Type cast(int16_t in) { return static_cast<Type>(in); }
	};

	template <>
    struct Expand<int32_t>
    {
		using Type = int64_t;
		inline static constexpr Type cast(int32_t in) { return static_cast<Type>(in); }
    };

	template <>
	struct Expand<uint8_t>
	{
		using Type = uint16_t;
		inline static constexpr Type cast(uint8_t in) { return static_cast<Type>(in); }
	};

	template <>
	struct Expand<uint16_t>
	{
		using Type = uint32_t;
		inline static constexpr Type cast(uint16_t in) { return static_cast<Type>(in); }
	};

	template <>
	struct Expand<uint32_t>
	{
		using Type = uint64_t;
		inline static constexpr Type cast(uint32_t in) { return static_cast<Type>(in); }
	};
}
