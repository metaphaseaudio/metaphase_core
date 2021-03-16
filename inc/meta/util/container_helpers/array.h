//
// created by mzapp 3/31/2020
//
#pragma once
#include <array>

namespace meta
{
	template <typename T, size_t Na, size_t Nb, size_t... AIs, size_t... BIs>
	static constexpr std::array<T, Na + Nb> concat_array_impl(const std::array<T, Na>& a, const std::array<T, Nb>& b,
	                                                   const std::index_sequence<AIs...>, const std::index_sequence<BIs...>)
	{ return { a[AIs]..., b[BIs]... }; }


	template <typename T, size_t Na, size_t Nb, typename AIs = std::make_index_sequence<Na>, typename BIs = std::make_index_sequence<Nb>>
	static constexpr std::array<T, Na + Nb> concat_array(const std::array<T, Na>& a, const std::array<T, Nb>& b)
	{ return concat_array_impl(a, b, AIs{}, BIs{}); }


	template <typename T, size_t N, size_t... Is>
	constexpr std::array<T, N> negate_array_impl(const std::array<T, N>& x, const std::index_sequence<Is...>)
	{ return std::array<T, N>({ -x[Is]... }); }


    template <typename T, size_t N, typename Is = std::make_index_sequence<N>>
	constexpr std::array<T, N> negate_array(const std::array<T, N>& x)
	{ return negate_array_impl<T, N>(x, Is{}); }



    template <typename T, std::size_t N, size_t... Is, typename... Args>
    constexpr std::array<T, N> make_initialized_array_impl(const std::index_sequence<Is...>&, Args&&... args)
    { return std::array<T, N>{ ((void)Is, T{args...})... }; }


    template <typename T, std::size_t N, typename... Args>
    constexpr std::array<T, N> make_initialized_array(Args&&... args)
    { return make_initialized_array_impl<T, N>(std::make_index_sequence<N>{}, args...); }

    template <typename T, size_t N, size_t... Is>
    constexpr std::array<std::pair<size_t, T>, N> enumerate_impl(const std::index_sequence<Is...>&, std::array<T, N>&& x)
	{
		return {std::make_pair(Is, x[Is])...};
	};

    template <typename T, size_t N>
    constexpr std::array<std::pair<size_t, T>, N> enumerate(std::array<T, N>&& x)
	{
    	return enumerate_impl<T, N>(std::make_index_sequence<N>{}, std::forward(x));
	}
}
