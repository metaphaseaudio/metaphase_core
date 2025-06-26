//
// Created by Matt on 6/23/2025.
//

#pragma once
//
// Created by Matt Zapp on 3/15/2020.
//

#pragma once

#include <meta/testing/TestBase.h>
#include <meta/testing/TestHelpers.h>
#include <meta/util/file/AudioFileHelpers.h>
#include <meta/dsp/WavetableReader.h>
#include <meta/dsp/BandlimitedWavetable.h>
#include "meta/dsp/DSPPrimitives.h"


class SharedMemoryBlockTest
    : public meta::TestBase
{
public:
    void tick(float* l, float* r)
    {
        auto c = m_DSP.next();
        auto lfo = 5.0f;
        auto inputGain = 1.0f;

        c.setValue(*l * inputGain);
        c.processAllPass<AP1L>(m_Diffusion);
        c.processAllPass<AP2L>(m_Diffusion);
        c.interpolateAndAccumulateWithScale<DlyL>(5, m_ReverbTime);
        c.lowPass(m_LPDecay1, m_LowPass);
        c.writeToBufferThenScaleBy<DlyL>(0, 1.0f);

        auto wetl = c.getValue();
        if ( wetl != 0)
        {
            std::cout << "hit!" << std::endl;
        }

        c.setValue(*r * inputGain);
        c.processAllPass<AP1R>(m_Diffusion);
        c.processAllPass<AP2R>(m_Diffusion);
        c.interpolateAndAccumulateWithScale<DlyR>(5, m_ReverbTime);
        c.lowPass(m_LPDecay2, m_LowPass);
        c.writeToBufferThenScaleBy<DlyR>(0, 1.0f);

        auto wetr = c.getValue();

        *l = wetl;
        *r = wetr;
    }

    void runReverb(int samples)
    {
        juce::AudioBuffer<float> buffer(2, samples);
        buffer.clear();
        auto x = buffer.getArrayOfWritePointers();
        x[0][0] = 0;
        x[1][0] = 1;

        for (int i = 0; i < samples; i++)
        {
            x[1][i] = 1;

            tick(x[0] + i, x[1] + i);
        }
    }
private:


    enum Stages {
        AP1L = 0,
        AP2L,
        AP1R,
        AP2R,
        DlyL,
        DlyR,
    };

    float m_Diffusion{0.625f};
    float m_ReverbTime{0.6f};
    float m_WetDry{0.5f};
    float m_LowPass{0.7f}, m_LPDecay1{}, m_LPDecay2{};

    meta::SharedMemoryBlock<float, 10, 10, 10, 10, 10, 10> m_DSP;
};


TEST_F(SharedMemoryBlockTest, reverb)
{
    runReverb(2000);
}
