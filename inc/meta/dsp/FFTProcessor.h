//
// Created by Matt on 8/14/2025.
//

#pragma once
#include <array>
#include <vector>
#include <juce_dsp/juce_dsp.h>
#include "../util/math.h"

namespace meta::dsp
{
    template <typename FloatType, size_t FFTOrder, size_t
        NOverlap, juce::dsp::WindowingFunction<FloatType>::WindowingMethod window>
    class FFTProcessor
    {
    public:
        static constexpr auto FFTSize = meta::static_power<2, FFTOrder>::value;
        using FFTFrameChannel = std::array<std::complex<FloatType>, FFTSize>;
        using FFTFrame = std::vector<FFTFrameChannel>;

        FFTProcessor()
            : m_FFT(FFTOrder)
            , m_WindowedInput{1, FFTSize}
        {}

        void process(const juce::AudioBuffer<FloatType>& in, FFTFrame& out)
        {
            jassert(in.getNumChannels() == out.size());
            jassert(in.getNumSamples() == FFTSize);


            for (int c = 0; c < in.getNumChannels(); c++)
            {
                m_WindowedInput.copyFrom(0, 0, in, c, 0, FFTSize);
                m_Window.multiplyWithWindowingTable(m_WindowedInput.getArrayOfWritePointers()[0], FFTSize);

                for (int s = 0; s < FFTSize; s++)
                    { m_Input[s] = m_WindowedInput.getArrayOfReadPointers()[0][s]; }

                m_FFT.perform(m_Input.data(), out[c].data(), false);
            }
        }

        void inverse(const FFTFrame& in, juce::AudioBuffer<FloatType>& out)
        {
            jassert(in.size() == out.getNumChannels());

            for (int c = 0; c < out.getNumChannels(); c++)
            {
                m_FFT.perform(in[c].data(), m_Input.data(), true);

                for (int s = 0; s < FFTSize; s++)
                    { out.getArrayOfWritePointers()[c][s] = m_Input[s].real(); }
            }


        }


        static void calculate_magnitudes(const FFTFrameChannel& frame, std::array<FloatType, FFTSize>& magnitudes)
        {

        }

    private:
        juce::dsp::FFT m_FFT;
        juce::AudioBuffer<FloatType> m_WindowedInput;
        juce::dsp::WindowingFunction<FloatType> m_Window{FFTSize, window};
        std::array<std::complex<FloatType>, FFTSize> m_Input;
    };
}
