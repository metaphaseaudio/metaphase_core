//
// Created by Matt Zapp on 4/8/2018.
//
//    A set of structures and functions to help make static, bandlimited
//    wavetables comprised of a specific number of partials with variable
//    weights.
//
//    To use, declare an array you wish to fill with wavetable data, and either
//    call one of the standard wavetype static functions, or build a function
//    that generates a series of partials which will fill the table. Tables do
//    not have to be static, but it can be useful to reduce binary sizes when a
//    wavetable will be used as an un-changing source of truth.
//

#pragma once

#include <array>
#include <meta/util/NumericConstants.h>
#include <meta/util/math.h>

#define GIBBS_CONSTANT 0.089489872236

namespace meta
{
    template <typename NumericType, size_t TableSize>
    struct WaveTable
    {
        NumericType sample_rate;
        std::array<NumericType, TableSize> table;
    };


    template <typename NumericType>
    static NumericType gibbsCompensationValue(NumericType partial_number, NumericType total_partials)
    {
        const auto x = (partial_number * meta::NumericConstants<NumericType>::PI) / total_partials;
        return sin(x) / x;
    }


    template <typename NumericType>
    constexpr static NumericType overshoot_compensation_coeff() { return 1.0f - (2.0f * GIBBS_CONSTANT); }


    /**
     * Calculates the gain coefficient for an arbitrary partial with a linear amplitude decay
     * @param harmonic the partial number to calculate
     * @return the gain of that partial in a saw or square wave
     */
    template <typename NumericType>
    static constexpr NumericType getSquarePartialGain(size_t k, NumericType n = 1)
        { return (pow(NumericType(1), NumericType(1) / n) / NumericType(k)) * overshoot_compensation_coeff<NumericType>(); }


    template <typename NumericType>
    static constexpr NumericType getSawPartialGain(size_t k, NumericType n = 1)
    {
        return 2 / (k * meta::NumericConstants<NumericType>::PI) * overshoot_compensation_coeff<NumericType>();
    }


    /**
     * Calculates the gain coefficient for an arbitrary partial
     * @param harmonic the partial number to calculate
     * @return the gain of that partial in a triangle wave
     */
    template <typename NumericType>
    static constexpr NumericType getTrianglePartialGain(size_t k)
    {
        return (8 / (pow(k * meta::NumericConstants<NumericType>::PI, 2))) * ((k + 3) % 4 ? -1 : 1);
    }


    template <typename NumericType, size_t TableSize>
    struct BandlimitedWavetable
    {
        /**
         * fills a table with a sine wave using a particular gain coefficient
         */
        template <int partial, int phase_num=0, int phase_denom=1>
        struct Partial
        {
            /**
             * Add this partial's values to an array
             * @param table the table to fill
             */
            static constexpr void addToTable(std::array<NumericType, TableSize>& table, NumericType gain)
            {
                for (int i = TableSize; --i >= 0;)
                {
                    // Cycle is 0 to 2pi times the harmonic number. Map this to
                    // the length of the table into which the partial is being
                    // inserted.
                    NumericType denominator = static_cast<NumericType>(i * 2 * partial)
                                            / static_cast<NumericType>(TableSize);
                    NumericType total_phase = denominator * meta::NumericConstants<NumericType>::PI +
                            phase_num * meta::NumericConstants<NumericType>::PI / NumericType(phase_denom);
                    table[i] += meta::sin(total_phase) * gain;
                }
            }
        };

////////////////////////////////////////////////////////////////////////////////////////////////////

        /**
         * Make a wavetable containing a single sine wave partial
         * @return an array containing the wavetable
         */
        static constexpr std::array<NumericType, TableSize> makeSin()
        {
            std::array<NumericType, TableSize> out{0};
            Partial<1>::addToTable(out, 1);
            return out;
        };


        static constexpr std::array<NumericType, TableSize> makeCosin()
        {
            std::array<NumericType, TableSize> out{0};
            Partial<1, 1, 2>::addToTable(out, 1);
            return out;
        };


        /**
         * Make a bandlimited square wave containing a specific number of
         * partials. Contains only odd harmonics.
         *
         * @param numHarmonics - the number of partials to be used in the wave
         * @param gibbsCompAmt - smooths the waveform to eliminate overshoot.
         *                       requires a value between 0 and 1;
         * @return an array containing the wavetable
         */
        static std::array<NumericType, TableSize> makeSquare (int numHarmonics)
        {
            std::array<NumericType, TableSize> out{0};
            for (int harmonic = 1; harmonic <= numHarmonics * 2; harmonic += 2)
            {
//                Partial<harmonic, getSquarePartialGain<NumericType>(harmonic, numHarmonics)>::addToTable(out);
            }

            return out;
        };


        /**
         * Make a bandlimited saw wave containing a specific number of
         * partials. Contains both odd and even harmonics.
         *
         * @param numHarmonics - the number of partials to be used in the wave
         * @param gibbsCompAmt - smooths the waveform to eliminate overshoot.
         *                       requires a value between 0 and 1;
         * @return an array containing the wavetable
         */
        static std::array<NumericType, TableSize> makeSaw(int numHarmonics)
        {
            std::array<NumericType, TableSize> out{0};
            for (int harmonic = 1; harmonic <= numHarmonics; harmonic++)
            {
//                Partial<harmonic, getSawPartialGain<NumericType>(harmonic, numHarmonics)>::addToTable(out);
            }

            return out;
        };


        /**
         * Make a band-limited triangle wave containing a specific number of
         * partials. Contains only odd harmonics.
         *
         * @param numHarmonics - the number of partials to be used in the wave
         * @param gibbsCompAmt - smooths the waveform to eliminate overshoot.
         *                       requires a value between 0 and 1;
         * @return an array containing the wavetable
         */
        static std::array<NumericType, TableSize> makeTriangle(int numHarmonics)
        {
            std::array<NumericType, TableSize> out{0};
            for (int harmonic = 1; harmonic <= numHarmonics * 2; harmonic += 2)
            {
//                Partial<harmonic, getTrianglePartialGain<NumericType>(harmonic)>::addToTable(out);
            }

            return out;
        };
    };
}
