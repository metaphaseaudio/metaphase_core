//
// Created by Matt on 4/26/2020.
//

#pragma once


#include <meta/dsp/BandlimitedWavetable.h>
#include <meta/dsp/WavetableOscillator.h>
#include <meta/generators/LinearRamp.h>

namespace meta
{
    template <typename NumericType, size_t TableSize>
    class SineSweep
        : public LinearRamp<NumericType>
    {
        std::array<NumericType, TableSize> m_SineTable;
        meta::WavetableOscilator<NumericType, TableSize> m_SineOsc;

    public:
        SineSweep(NumericType startFreq, NumericType endFreq, size_t samples)
            : LinearRamp<NumericType>(startFreq, endFreq, samples)
            , m_SineTable(meta::BandlimitedWavetable<NumericType, TableSize>::makeSin())
            , m_SineOsc(m_SineTable)
        {}

        NumericType tick()
        {
            m_SineOsc.setFrequency(LinearRamp<NumericType>::tick());
            return m_SineOsc.tick();
        }
    };    
}
