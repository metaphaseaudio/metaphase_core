//
// Created by Matt on 4/26/2020.
//

#pragma once


#include <meta/dsp/BandlimitedWavetable.h>
#include <meta/dsp/WavetableReader.h>
#include <meta/generators/LinearRamp.h>

namespace meta
{
    template <typename NumericType, size_t TableSize>
    class SineSweep
        : public LinearRamp<NumericType>
    {
        std::array<NumericType, TableSize> m_SineTable;
        meta::WavetableState<NumericType> m_State;
        meta::WavetableReader<NumericType, TableSize> m_SineOsc;
        NumericType m_SampleRate;

    public:
        SineSweep(NumericType sampleRate, NumericType startFreq, NumericType endFreq, size_t samples)
            : LinearRamp<NumericType>(startFreq, endFreq, samples)
            , m_SineTable(meta::BandlimitedWavetable<NumericType, TableSize>::makeSin())
            , m_SineOsc(m_SineTable, m_State)
            , m_SampleRate(sampleRate)
        {}

        NumericType tick()
        {
            m_SineOsc.setFrequency(m_SampleRate, LinearRamp<NumericType>::tick());
            return m_SineOsc.tick();
        }
    };    
}
