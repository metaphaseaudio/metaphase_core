//
// Created by Matt on 3/15/2022.
//

#pragma once
#include <array>
#include <meta/util/math.h>

namespace meta
{
    template <typename DType, int N, double OverSample, double treble, double cutoff>
    constexpr std::array<DType, N> makeSincTable()
    {
        std::array<DType, N> out;
        double const maxh = 4096.0;
        double const rolloff = pow( 10.0, 1.0 / (maxh * 20.0) * treble / (1.0 - cutoff) );
        double const pow_a_n = pow( rolloff, maxh - maxh * cutoff );
        double const to_angle = meta::NumericConstants<DType>::PI / 2 / maxh / OverSample;
        for ( int i = 0; i < N; i++ )
        {
            double angle = ((i - N) * 2 + 1) * to_angle;
            double c = rolloff * meta::cos( (maxh - 1.0) * angle ) - meta::cos( maxh * angle );
            double cos_nc_angle = meta::cos( maxh * cutoff * angle );
            double cos_nc1_angle = meta::cos( (maxh * cutoff - 1.0) * angle );
            double cos_angle = meta::cos( angle );

            c = c * pow_a_n - rolloff * cos_nc1_angle + cos_nc_angle;
            double d = 1.0 + rolloff * (rolloff - cos_angle - cos_angle);
            double b = 2.0 - cos_angle - cos_angle;
            double a = 1.0 - cos_angle - cos_nc_angle + cos_nc1_angle;

            out[i] = (float) ((a * d + c * b) / (b * d));
        }
    }
}
