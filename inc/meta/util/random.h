//
// Created by Mattb on 7/21/2022.
//

#pragma once
#include <random>

namespace meta
{
    template <typename NumType, typename Distribution = std::uniform_int_distribution<NumType>>
    class RandomRangeGenerator
    {
    public:
        RandomRangeGenerator(NumType start, NumType end)
            : generator(device())
            , distribution(start, end)
        {}

        NumType next() { return distribution(generator); }
    private:
        std::random_device device;
        std::mt19937 generator;
        Distribution distribution;
    };
}