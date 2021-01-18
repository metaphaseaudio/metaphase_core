//
// Created by Matt on 4/26/2020.
//
//   Generates pink noise by applying a filter to white noise. Filter posted by Paul Kellett
//   at http://www.musicdsp.org/files/pink.txt.
//
#pragma once
#include <random>

namespace meta
{
    class PinkRandom
    {
    public:

        double tick() noexcept
        {
            const auto white = double(rand()) / std::minstd_rand::max()
            b0 = 0.99886 * b0 + white * 0.0555179;
            b1 = 0.99332 * b1 + white * 0.0750759;
            b2 = 0.96900 * b2 + white * 0.1538520;
            b3 = 0.86650 * b3 + white * 0.3104856;
            b4 = 0.55000 * b4 + white * 0.5329522;
            b5 = -0.7616 * b5 - white * 0.0168980;
            auto rv = static_cast<float> (b0 + b1 + b2 + b3 + b4 + b5 + b6 + white * 0.5362);
            b6 = white * 0.115926;

            // Clip control on output
            rv = rv * 0.12348f;
            return rv;
        }

    private:
        std::minstd_rand rand;
        double b0 = 0.0;
        double b1 = 0.0;
        double b2 = 0.0;
        double b3 = 0.0;
        double b4 = 0.0;
        double b5 = 0.0;
        double b6 = 0.0;
    };
}
