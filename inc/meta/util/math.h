//
// Created by Matt Zapp on 4/28/2018.
//

#pragma once

#include <cmath>
#include <type_traits>
#include <limits>
#include <array>
#include <cmath>
#include <iostream>
#include "NumericConstants.h"


namespace meta
{
    template <typename Type, Type Value>
    struct static_negate { static constexpr Type value = -Value; };


    template <typename NumericType>
    NumericType wrap(NumericType x, NumericType lower, NumericType upper)
    {
        auto range = upper - lower + 1;

        if (x < lower)
            { x += range * ((lower - x) / range + 1); }

        return lower + fmodf(x - lower, range);
    }

    template <typename NumericType>
    constexpr NumericType limit(NumericType low, NumericType high, NumericType value)
    { return value < low ? low : (high < value ? high : value); }

	template <typename T>  constexpr T abs(T i){ return i < 0 ? -1 * i : i; }
	template <typename T>  constexpr T fabs(T i) { return std::fabs(i); }

    template <typename NumericType>
    struct Interpolate
    {
        static constexpr NumericType linear(NumericType a, NumericType b, NumericType weight)
            { return (b * weight) + (a * (1 - weight)); }

        static constexpr NumericType parabolic
        (NumericType a, NumericType b, NumericType weight, NumericType exponent=2)
        {
            if (fabs(exponent) < 0.001) { return linear(a, b, weight); }
            const auto ratio = (1 - exp(exponent * weight)) / (1 - exp(exponent));
            return a + (b - a) * ratio;
        }
    };

	template <typename NumericType>
	constexpr NumericType power(NumericType base, size_t exponent)
	    { return exponent == 0 ? 1 : base * power(base, exponent - 1);}


    template <size_t base, size_t exponent>
	struct static_power {
	    static constexpr size_t value = base * static_power<base, exponent - 1>::value;
	};

    template <size_t base> struct static_power<base, 0> { static constexpr size_t value = 1; };

    template <typename NumericType, NumericType in>
    struct static_abs {
        static constexpr NumericType value = (in >= 0) ? in : - in;
    };

    template<std::size_t... Is> struct seq{};

    template<std::size_t N, std::size_t... Is>
    struct gen_seq: gen_seq<N-1, N, Is...> {};

    template<std::size_t... Is>
    struct gen_seq<0, Is...> : seq<Is...>{};



//    constexpr static double pi_v = pi<double>;
//    constexpr static double two_pi_v = two_pi<double>;
//    constexpr static double half_pi_v = half_pi<double>;

    template<class T,class dcy = std::decay_t<T>>
    constexpr inline std::enable_if_t<std::is_floating_point<T>::value,dcy> inverse(T value){
        return (value == 0) ? 0.0 : 1.0 / value;
    }
    constexpr inline long double factorial(std::intmax_t const& n){
        if(n==0){return 1;}
        long double result = n;
        for(std::intmax_t i=n-1;i>0;--i){
            result *=i;
        }
        return result;
    }
    constexpr inline std::size_t max_factorial(){
        std::size_t i=0;
        long double d=0;
        while ((d= factorial(i))<std::numeric_limits<long double>::max()){++i;}
        return i;
    }

    template<class base,std::size_t N>
    class trig_coeffs {
        using T = typename base::value_type;
        using array_type = std::array<T,N>;

        template<std::size_t ... NS>
        constexpr static inline array_type _coeffs(seq<NS ...>){
            return {{base::coeff(NS) ...}};
        }
    public:
        constexpr static array_type coeffs=_coeffs(gen_seq<N>{});
    };
    template<class base,std::size_t N>
    constexpr typename trig_coeffs<base,N>::array_type trig_coeffs<base,N>::coeffs;


    template<class base,std::size_t N, class dcy = std::decay_t<typename base::value_type>>
    constexpr std::enable_if_t<std::is_floating_point<dcy>::value,dcy>
    _sincos(typename base::value_type x) noexcept{
        using c = trig_coeffs<base,N>;

        dcy result = 0.0;//result accumulator
        //do input range mapping
        dcy _x =base::range_reduce(x);
        //taylor series
        {
            const dcy x_2 = _x*_x; //store x^2
            dcy pow = base::initial_condition(_x);
            for(auto&& cf: c::coeffs){
                result +=  cf * pow;
                pow*=x_2;
            }
        }
        return result;
    }
    namespace detail{
        template<class T>
        struct _sin{
            using value_type = T;
            constexpr static inline T coeff(std::size_t n)noexcept  {
                return (n % 2 ? 1 : -1) * inverse(factorial((2 * n)-1));
            }
            constexpr static inline T range_reduce(T x)noexcept{
                T _x = x;
                _x += NumericConstants<T>::PI;
                _x -= static_cast<std::size_t>(_x / NumericConstants<T>::TWO_PI) * NumericConstants<T>::TWO_PI;
                _x -= NumericConstants<T>::PI;
                return _x;
            }
            constexpr static inline T initial_condition(T x)noexcept{
                return x;
            }
            constexpr static inline std::size_t default_N()noexcept{
                return 16;
            }
        };
    }

    template<class T,std::size_t N = detail::_sin<T>::default_N()>
    constexpr inline std::decay_t<T> sin(T x) noexcept {
        return _sincos<detail::_sin<T>,N>(x);
    }

    template<class T>
    constexpr inline std::decay_t<T> cos(T x) noexcept { return sin(x - meta::NumericConstants<T>::PI);}
}
