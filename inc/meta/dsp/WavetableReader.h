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
    };


    template<typename NumericType, size_t TableSize>
    class WavetableReader
    {
    public:
        explicit WavetableReader(std::array<NumericType, TableSize>& wavetable, WavetableState<NumericType> state)
            : r_Wavetable(wavetable)
            , r_State(state)
        {}

        void reset(NumericType phase = 0) { r_State.phase = phase; }

        void setFrequency(NumericType sample_rate, NumericType freq)
            { r_State.delta = WavetableHelpers<NumericType>::calculate_delta(TableSize, sample_rate, freq); }

        inline NumericType tick()
        {
            r_State.phase += r_State.delta;
            const auto i = static_cast<int>(r_State.phase) % TableSize;
            const auto f = r_State.phase - static_cast<int>(r_State.phase);
            const auto j = (i == TableSize - 1) ? 0 : i + 1;
            const auto a = r_Wavetable.at(i);
            const auto b = r_Wavetable.at(j);
            // If we looped, this will keep things within table bounds.
            r_State.phase = i + f;
            return (a * (1 - f)) + (b * f);
        }

    private:
        std::array<NumericType, TableSize>& r_Wavetable;
        WavetableState<NumericType>& r_State;
    };

}