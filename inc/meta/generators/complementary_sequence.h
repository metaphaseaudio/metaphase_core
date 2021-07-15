//
// created by mzapp 3/31/2020
//
#pragma once
#include <meta/util/math.h>
#include <meta/util/container_helpers/array.h>
#include <type_traits>

namespace meta
{
    template <size_t I>
    using complementary_sequence = std::tuple<const std::array<signed, I>, const std::array<signed, I>>;

    template<size_t I>
    static constexpr complementary_sequence<2 * I> extend_sequence(const complementary_sequence<I>& last_sequence)
    {
        return std::make_tuple(
                concat_array(std::get<0>(last_sequence), std::get<1>(last_sequence)),
                concat_array(std::get<0>(last_sequence), negate_array(std::get<1>(last_sequence))));
    }

	template <size_t N>
	struct Golay
	{
        static constexpr size_t size = static_power<2, N>::value;
		static constexpr complementary_sequence<size> value = extend_sequence(Golay<N - 1>::value);		
	};	

	template <>
	struct Golay<0>
	{
        static constexpr size_t size = 1;
		static constexpr complementary_sequence<size> value = std::make_tuple(std::array<signed, 1>{ 1 }, std::array<signed, 1>{ 1 });
	};

	template <typename NumType=float>
	static std::pair<std::vector<NumType>, std::vector<NumType>> generate_golay_dynamic(int golay_n)
    {
	    std::vector<NumType> a{1, 1};
        std::vector<NumType> b{1, -1};

        for (int i = golay_n; --i > 0;)
        {
            // copy A for later, and append B
            auto last_a = a;
            a.insert(a.end(), b.begin(), b.end());

            // invert B, save it appended to the last A
            juce::FloatVectorOperations::negate(&*b.begin(), &*b.begin(), b.size());
            last_a.insert(last_a.end(), b.begin(), b.end());
            b = last_a;
        }

        return std::make_pair(a, b);
    }
}