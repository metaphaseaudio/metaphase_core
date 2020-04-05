//
// created by mzapp 3/31/2020
//
#pragma once
#include <array>

namespace meta
{
	template <typename T, size_t Na, size_t Nb, size_t... AIs, size_t... BIs>
	constexpr std::array<T, Na + Nb> 
		concat_array_impl(const std::array<T, Na>& a, const std::array<T, Nb>& b,
		                                               std::index_sequence<AIs...>, std::index_sequence<BIs...>)
	{ return { a[AIs]..., b[BIs]... }; }

	template <typename T, size_t Na, size_t Nb, typename AIs = std::make_index_sequence<Na>, typename BIs = std::make_index_sequence<Nb>>
	constexpr std::array<T, Na + Nb> 
		concat_array(const std::array<T, Na>& a, const std::array<T, Nb>& b)
	{ return concat_array_impl(a, b, AIs{}, BIs{}); }
	

	template <typename T, size_t N, size_t... Is>
	constexpr std::array<T, N> 
		negate_array_impl(const std::array<T, N>& x, std::index_sequence<Is...>)
	{ return { -x[Is]... }; }


	template <typename T, size_t N, typename Is = std::make_index_sequence<N>>
	constexpr std::array<T, N> 
		negate_array(const std::array<T, N>& x)
	{ return negate_array_impl(x, Is{}); }
}
