#include "inc/meta/audio/Asymp.h"
#include <cmath>

using namespace meta;


Asymp::Asymp()
    : m_Value(0.0f)
    , m_Target(0.0f)
    , m_Factor(0.01f)
    , m_Constant(0.0f)
{}

Asymp::Asymp(double sampleRate)
    : m_Value(0.0f)
    , m_Target(0.0f)
    , m_Factor(float(exp(-1.0 / (0.3 * sampleRate))))
    , m_Constant(0.0f)
{}


void Asymp::init(double sampleRate)
{
    m_Value = 0.0;
    m_Target = 0.0;
    m_Factor = float(exp(-1.0 / (0.3 * sampleRate)));
    m_Constant = 0.0;
}

void Asymp::setTau(float tau, float sampleRate)
{
    if (tau <= 0.0) { return; }

    m_Factor = std::exp(-1.0f / (tau * sampleRate));
    m_Constant = (1.0f - m_Factor) * m_Target;
}

void Asymp::setTime(float time, float sampleRate)
{
    if (time <= 0.0) { return; }

    float tau = -time / std::log(TARGET_THRESHOLD);
    setTau(tau, sampleRate);
}

void Asymp::setT60(float t60, float sampleRate)
{
    if (t60 <= 0.0) { return; }
    setTau(t60 / 6.91f, sampleRate);
}

void Asymp::setTarget(float target)
{
    m_Target = target;
    m_Constant = (1.0f - m_Factor) * m_Target;
}

void Asymp::setValue(float value)
{
    m_Value = value;
}

float Asymp::getValue() const
{
    return m_Value;
}

