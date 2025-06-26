//
// Created by Matt on 6/22/2025.
//

#pragma once
#include <random>
#include "juce_core/juce_core.h"

namespace meta
{

class SmoothedSAH
{
public:
    explicit SmoothedSAH(float freq, float sampleRate);
    void setFreq(float freq);
    [[ nodiscard ]] float getFreq() const;
    float tick();
    [[ nodiscard ]] float getValue() const { return m_Value; };
    void setTarget(float target);

private:
    float m_Value{0}, m_SampleRate, m_StepSize{}, m_SamplesToTarget{}, m_Target{0};
    int m_Countdown{0};
    juce::Random m_Rand;
};

};
