//
// Created by Matt Zapp on 3/15/2020.
//

#pragma once


#include <meta/audio/SingletonSampleRate.h>
#include "BandlimitedWavetable.h"


namespace meta
{
    template <typename NumericType, size_t TableSize>
    class WavetableOscilator
    {
    public:
        explicit WavetableOscilator(std::array<NumericType, TableSize>& wavetable)
            : r_Wavetable(wavetable)
            , m_Delta(0)
            , m_Phase(0)
        {}

        void reset(NumericType phase=0) { m_Phase = phase; }

        void setFrequency(NumericType freq)
        {
            auto sampleRate = meta::SingletonSampleRate<float>::getValue();
            m_Delta = std::fmod(static_cast<float>(TableSize) * freq / sampleRate, TableSize);
            if (m_Delta < 0) { m_Delta = TableSize + m_Delta; }  // Add here because signs are inverted.
        }

        inline NumericType tick()
        {
            m_Phase += m_Delta;
			const auto i = static_cast<int>(m_Phase) % TableSize;
            const auto f = m_Phase - static_cast<int>(m_Phase);
            const auto j = (i == TableSize - 1) ? 0 : i + 1;
            const auto a = r_Wavetable[i];
            const auto b = r_Wavetable[j];
            m_Phase = i + f;
            return (a * (1 - f)) + (b * f);
        }

    private:
        std::array<NumericType, TableSize>& r_Wavetable;
        NumericType m_Delta;
        NumericType m_Phase;
    };
}