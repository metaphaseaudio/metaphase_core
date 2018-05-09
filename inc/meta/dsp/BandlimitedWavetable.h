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

namespace meta
{
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
                    NumericType denominator = static_cast<NumericType>(i * 2 * partial)
                                            / static_cast<NumericType>(TableSize);
                    NumericType phase = denominator * meta::NumericConstants<NumericType>().PI;

                    table[i] += sin(phase) * gain;
                }
            }

            const int partial;
            const NumericType gain;
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
            gibbsCompAmt = meta::NumericConstants<NumericType>().PI * gibbsCompAmt
                         / static_cast<float>(numHarmonics);

            for (int harmonic = startingHarmonic; harmonic <= numHarmonics; harmonic += 2)
            {
                auto gibbsComp = 0.54 + 0.46 * cos ((harmonic - 0.5 / 30) * gibbsCompAmt);
                Partial part(harmonic, gibbsComp / static_cast<NumericType>(harmonic));
                part.addToTable(out);
            }

            return out;
        };

        /**
         * Calculates the gain coefficient for an arbitrary partial
         * @param harmonics the partial number to calculate
         * @return the gain of that partial in a saw or square wave
         */
        static NumericType getPartialGain
                (size_t harmonics, NumericType numHarmonics = 1.0f, NumericType gibbsCompAmt = 1.0f)
        {
            gibbsCompAmt = meta::NumericConstants<NumericType>().PI * gibbsCompAmt
                           / static_cast<float>(numHarmonics);

            auto gibbsComp = 0.54 + 0.46 * cos ((harmonics - 0.5 / 30) * gibbsCompAmt);
            return NumericType(1) / NumericType(harmonics);
        }

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
            gibbsCompAmt = meta::NumericConstants<NumericType>().PI * gibbsCompAmt
                         / static_cast<float>(numHarmonics);

            for (int harmonic = startingHarmonic; harmonic <= numHarmonics; harmonic++)
            {
                auto gibbsComp = 0.54 + 0.46 * cos ((harmonic - 0.5 / 30) * gibbsCompAmt);
                Partial part(harmonic, gibbsComp / static_cast<float>(harmonic));
                part.addToTable(out);
            }

            return out;
        };
    };
}