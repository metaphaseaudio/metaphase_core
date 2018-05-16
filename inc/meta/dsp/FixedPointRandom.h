/**
 *  Created by mzapp on 4/19/18.
 *
 *  Adapted from the Texas Instruments TMS320C5 application note:
 *  http://www.ti.com/lit/an/spra239/spra239.pdf
 */

#pragma once

#include <meta/util/WordSize.h>

namespace meta
{
    /**
     * Can generate a well-distributed set of pseudo-random numbers.
     * @tparam IntType any Unsigned Integer type
     */
    template <typename IntType>
    class FixedPointRandom
    {
    public:
        /**
         * Calculates the optimal increment value from the equation:
         * (.5 - (0.1666 * sqrt(3)) * modulusOfWord
         * @return the increment value
         */
        static constexpr IntType makeIdealIncrement()
        {
            return static_cast<IntType>(
                    (0.5f - ((1.0f / 6.0f) * 1.732050808f))
                 * (1l << meta::WordSizeInBits<IntType>::Value));
        }

        /**
         * Initialize a FixedPointRandom number generator.  Care must be taken
         * when selecting the variables below to ensure that an even
         * distribution of numbers is genrated.
         * @param seed - Can be any arbitrary value
         * @param mult - should be chosen such that the last three digits are
         *               even-2-1 (such as xx821,x421, etc.)
         * @param increment - any prime number related to the modulus of the
         *              width of the word. The following equation is suggested:
         *              (.5 - (0.1666 * sqrt(3)) * modulusOfWord
         */
        FixedPointRandom(IntType seed=21845, IntType mult=31821, IntType increment=makeIdealIncrement())
            : m_Value(seed)
            , m_Inc(increment)
            , m_Mult(mult)
            , m_LastValue(seed)
        {}

        IntType next()
        {
            m_LastValue = m_Value;
            m_Value = (m_LastValue * m_Mult) + m_Inc;
            return m_Value;
        }

        IntType getCurrentValue() const { return m_Value; }

    private:
        IntType m_Value;
        IntType m_LastValue;
        const IntType m_Mult;
        const IntType m_Inc;
    };

}
