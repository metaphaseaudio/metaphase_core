//
// created by mzapp 3/31/2020
//
#pragma once
#include <meta/util/math.h>
#include <meta/util/container_helpers/array.h>

namespace meta
{
	template <size_t I>
	using ab_sequence = std::tuple<std::array<char, I>, std::array<char, I>>;

	template <size_t N>
	struct Golay
	{
		template<size_t I>
		static constexpr ab_sequence<2 * I> extend_sequence(const ab_sequence<I>& last_sequence)
		{
			return std::make_tuple(
				concat_array(std::get<0>(last_sequence), std::get<1>(last_sequence)),
				concat_array(std::get<0>(last_sequence), negate_array(std::get<1>(last_sequence))));
		}
	
		static constexpr const ab_sequence<power(2, N)>
			generate() 
		{ 
			return extend_sequence(Golay<N - 1>::generate());
		}
	};	

	template <>
	struct Golay<0>
	{
		static constexpr const ab_sequence<1>
			generate()
		{ return std::make_tuple(std::array<char, 1>{ 1 }, std::array<char, 1>{ 1 }); }
	};
}