//
// created by mzapp 3/22/2020
//
#pragma once
#include <tuple>
#include <vector>

namespace meta
{

	template <typename T>
	std::tuple<std::vector<T>, std::vector<T>> split(const std::vector<T>& x, size_t index)
	{
		return std::make_tuple(std::vector<T>(x.begin(), x.begin() + index),
			                   std::vector<T>(x.begin() + index, x.end()));
	}

	template <typename T>
	std::vector<T> slice(const std::vector<T>& x, size_t begin, size_t end)
	{
		if (begin > end)
		{
			auto rv = std::vector<T>(x.begin() + end, x.begin() + begin);
			std::reverse(rv.begin(), rv.end());
			return rv;
		}			
		return std::vector<T>(x.begin() + begin, x.begin() + end);
	}
}
