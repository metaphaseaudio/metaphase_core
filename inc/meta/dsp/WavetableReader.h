//
// Created by Matt Zapp on 3/15/2020.
//

#pragma once


#include <meta/audio/SingletonSampleRate.h>
#include "BandlimitedWavetable.h"


namespace meta
{
    template <typename NumericType>
    struct WavetableState
    {
        NumericType delta = 0;
        NumericType phase = 0;
    };

    template <typename NumericType>
    struct WavetableHelpers
    {
        inline static NumericType calculate_delta(NumericType table_size, NumericType sample_rate, NumericType freq)
        {
            auto delta = std::fmod(static_cast<NumericType>(table_size) * freq / sample_rate, table_size);
            return (delta < 0) ? table_size + delta : delta;  // Add here because signs are inverted.
        };

        using IFJ = std::tuple<size_t, NumericType, size_t>;

        inline static constexpr IFJ
            calculateIFJ(const size_t table_size, const NumericType phase)
        {
            auto i = static_cast<int>(phase) % table_size;
            auto f = phase - static_cast<int>(phase);
            auto j = (i == table_size - 1) ? 0 : i + 1;
            return std::make_tuple(i,f, j);
        }

        inline static constexpr NumericType calculate_sample(NumericType* data, const IFJ& ifj)
        {
            return meta::Interpolate<NumericType>::linear(
                data[std::get<0>(ifj)],
                data[std::get<2>(ifj)],
                std::get<1>(ifj)
            );
        }
    };


    template<typename NumericType, size_t TableSize>
    class WavetableReader
    {
    public:
        explicit WavetableReader(std::array<NumericType, TableSize>& wavetable, WavetableState<NumericType>& state)
            : r_Wavetable(wavetable)
            , r_State(state)
        {}

        void reset(NumericType phase = 0) { r_State.phase = phase; }

        void setFrequency(NumericType sample_rate, NumericType freq)
            { r_State.delta = WavetableHelpers<NumericType>::calculate_delta(TableSize, sample_rate, freq); }

        inline NumericType tick()
        {
            r_State.phase += r_State.delta;
            const auto ifj = WavetableHelpers<NumericType>::calculateIFJ(TableSize, r_State.phase);
            const auto a = r_Wavetable.at(std::get<0>(ifj));
            const auto b = r_Wavetable.at(std::get<2>(ifj));
            // If we looped, this will keep things within table bounds.
            r_State.phase = std::get<0>(ifj) + std::get<1>(ifj);
            return meta::Interpolate<NumericType>::linear(a, b, std::get<1>(ifj));
        }

    private:
        std::array<NumericType, TableSize>& r_Wavetable;
        WavetableState<NumericType>& r_State;
    };

}