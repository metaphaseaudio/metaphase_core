//
// Created by Matt on 8/2/2020.
//

#pragma once
#include <meta/generators/complementary_sequence.h>
#include <meta/dsp/FindInSignal.h>

namespace meta
{
namespace dsp
{
    template <typename NumType>
    struct IRCalc
    {
        template <typename T>
        static T calc_norm_factor(const T* x, size_t n)
        {
            using Mode = meta::simd<T>::ModeType<sizeof(*x)>::Mode;
            const int nSIMDOps = n / Mode::numParallel;

            const auto xLoad = (meta::simd<T>::isAligned(x)) ? Mode::load_aligned : Mode::load_unaligned;

            Mode::ParallelType x_pow = Mode::load_one(0);

            for (int i = 0; i < nSIMDOps; i++)
            {
                const Mode::ParallelType xp = xLoad(x);
                x_pow = Mode::hadd(x_pow, Mode::mul(xp, xp));
                x += (16 / sizeof(*x));
            }

            T tmp[4];
            const auto tmpStore = (meta::simd<T>::isAligned(tmp)) ? Mode::store_aligned : Mode::store_unaligned;

            tmpStore(tmp, x_pow);
            T x_sum = tmp[0] + tmp[1] + tmp[2] + tmp[3];

            n &= (Mode::numParallel - 1);

            if (n == 0) { return x_sum; }
            for (int i = 0; i < n; ++i) { x_sum += x[i] * x[i]; }

            return x_sum;
        }
    };
}
}