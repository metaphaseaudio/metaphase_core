//
// Created by Matt on 6/22/2025.
//

#include "meta/audio/SmoothedSAH.h"
#include "meta/dsp/BandlimitedWavetable.h"

using namespace meta;

static constexpr auto cosine = BandlimitedWavetable<float, 1024>::makeCosin();

SmoothedSAH::SmoothedSAH(float freq, float sampleRate)
    : m_SampleRate(sampleRate)
{
    setFreq(freq);
    setTarget(m_Rand.nextFloat() * 0.5f);
}

void SmoothedSAH::setFreq(float freq)
{
    m_SamplesToTarget = m_SampleRate / freq;
    setTarget(m_Target);
}

float SmoothedSAH::getFreq() const
{
    return m_SampleRate / m_SamplesToTarget;
}

float SmoothedSAH::tick()
{
    m_Value += m_StepSize;
    if (--m_Countdown <= 0)
    {
        m_Countdown = floor(m_SamplesToTarget);
        setTarget(m_Rand.nextFloat());
    }

    return cosine[static_cast<size_t>(m_Value * 512.0f)];
}

void SmoothedSAH::setTarget(float target)
{
    m_Target = target;
    m_StepSize = (target - m_Value) / m_SamplesToTarget;
}