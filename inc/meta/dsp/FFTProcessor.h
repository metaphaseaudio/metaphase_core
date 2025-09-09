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
    template <typename FloatType, size_t FFTOrder, typename juce::dsp::WindowingFunction<FloatType>::WindowingMethod Window>
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

                auto input = reinterpret_cast<float*>(out[c].data());
                std::memcpy(input, m_WindowedInput.getArrayOfReadPointers()[0], sizeof(float) * FFTSize);

                m_FFT.performRealOnlyForwardTransform(input, false);
            }
        }

        void inverse(const FFTFrame& in, juce::AudioBuffer<FloatType>& out)
        {
            jassert(in.size() == out.getNumChannels());

            for (int c = 0; c < out.getNumChannels(); c++)
            {
                auto input = reinterpret_cast<float*>(m_Tmp.data());
                std::memcpy(m_Tmp.data(), in[c].data(), sizeof(std::complex<float>) * FFTSize);
                m_WindowedInput.getArrayOfWritePointers()[c];
                m_FFT.performRealOnlyInverseTransform(input);

                std::memcpy(out.getArrayOfWritePointers()[c], input, sizeof(float) * FFTSize);

                m_Window.multiplyWithWindowingTable(out.getArrayOfWritePointers()[c], FFTSize);
            }
        }


        static void calculate_magnitudes(const FFTFrameChannel& frame, std::array<FloatType, FFTSize>& magnitudes)
        {

        }

    private:
        juce::dsp::FFT m_FFT;
        juce::AudioBuffer<FloatType> m_WindowedInput;
        juce::dsp::WindowingFunction<FloatType> m_Window{FFTSize, Window, true};
        std::array<std::complex<FloatType>, FFTSize> m_Tmp;
    };
}
