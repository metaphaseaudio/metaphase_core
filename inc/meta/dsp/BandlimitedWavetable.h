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

#define GIBBS_CONSTANT 0.089489872236

namespace meta
{
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
    static constexpr NumericType getLinearPartialGain
    (size_t harmonic, NumericType numHarmonics = 1.0f, NumericType gibbsCompAmt = 0.5f)
    {
        return (NumericType(1) / NumericType(harmonic)) * gibbsCompensationValue<NumericType>(harmonic, numHarmonics);
    }

    /**
     * Calculates the gain coefficient for an arbitrary partial
     * @param harmonic the partial number to calculate
     * @return the gain of that partial in a triangle wave
     */
    template <typename NumericType>
    static constexpr NumericType getTrianglePartialGain
            (size_t harmonic, size_t n_harmonics, NumericType gibbsCompAmt = 0.5f)
    {
        const auto on = (harmonic % 2 == 1) ? 8.0f / std::pow(meta::NumericConstants<NumericType>::PI, 2) : 0;
        const auto inv = (harmonic + 3) % 4;
        const auto mult = on * NumericType(inv == 0 ? -1 : 1);
        return  (mult / pow(harmonic, 2)) * gibbsCompensationValue<NumericType>(harmonic, n_harmonics / 2);
    }

    template <typename NumericType, size_t TableSize>
    struct BandlimitedWavetable
    {

        /**
         * fills a table with a sine wave using a particular gain coefficient
         */
        struct Partial
        {
            Partial(int p, NumericType g)
                : partial(p)
                , gain(g)
            {};

            /**
             * Add this partial's values to an array
             * @param table the table to fill
             */
            void addToTable(std::array<NumericType, TableSize>& table) const
            {
                for (int i = TableSize; --i >= 0;)
                {
                    // Cycle is 0 to 2pi times the harmonic number. Map this to
                    // the length of the table into which the partial is being
                    // inserted.
                    NumericType denominator = static_cast<NumericType>(i * 2 * partial)
                                            / static_cast<NumericType>(TableSize);
                    NumericType phase = denominator * meta::NumericConstants<NumericType>::PI;
                    table[i] += sin(phase) * gain;
                }
            }

            const int partial;
            const NumericType gain;
        };

////////////////////////////////////////////////////////////////////////////////////////////////////

        /**
         * Make a wavetable containing a single sine wave partial
         * @return an array containing the wavetable
         */
        static constexpr std::array<NumericType, TableSize> makeSin()
        {
            std::array<NumericType, TableSize> out{0};
            Partial part(1, 1);
            part.addToTable(out);
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
        static std::array<NumericType, TableSize> makeSquare
        (int numHarmonics, int startingHarmonic = 1, float gibbsCompAmt = 1.0f)
        {
            std::array<NumericType, TableSize> out{0};
            for (int harmonic = startingHarmonic; harmonic <= numHarmonics; harmonic += 2)
            {
                Partial part(harmonic, getLinearPartialGain<NumericType>(harmonic, numHarmonics, gibbsCompAmt));
                part.addToTable(out);
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
        static std::array<NumericType, TableSize> makeSaw
        (int numHarmonics, int startingHarmonic = 1, float gibbsCompAmt = 1.0f)
        {
            std::array<NumericType, TableSize> out{0};
            const auto overshoot_comp = 1.0f - (2.0f * GIBBS_CONSTANT);
            for (int harmonic = startingHarmonic; harmonic <= numHarmonics; harmonic++)
            {
                auto b = (-2.0f / (meta::NumericConstants<NumericType>::PI  * harmonic)) * (std::pow(-1.0f, harmonic));
                Partial part(harmonic, b * overshoot_comp * gibbsCompensationValue<NumericType>(harmonic, numHarmonics));// * getLinearPartialGain<NumericType>(harmonic, numHarmonics, gibbsCompAmt));
                part.addToTable(out);
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
        static std::array<NumericType, TableSize> makeTriangle
                (int numHarmonics, int startingHarmonic = 1, float gibbsCompAmt = 0.0f)
        {
            std::array<NumericType, TableSize> out{0};
            auto const_gain = 1.0f - (1.0f / numHarmonics);
            for (int harmonic = startingHarmonic; harmonic <= numHarmonics; harmonic += 2)
            {
                Partial part(harmonic, getTrianglePartialGain<NumericType>(harmonic, numHarmonics, gibbsCompAmt));
                part.addToTable(out);
            }

            return out;
        };
    };
}
