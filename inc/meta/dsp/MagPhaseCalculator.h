//
// Created by Matt on 7/25/2021.
//

#pragma once
#include <juce_dsp/juce_dsp.h>


namespace meta
{
namespace dsp
{
    template <typename T, size_t order>
    class MagPhaseCalculator
    {
    public:
        constexpr static auto fft_size = meta::static_power<2, order>::value;

        MagPhaseCalculator()
                : m_FFT(order)
                , m_Window(fft_size, juce::dsp::WindowingFunction<T>::hann)
                , m_Fifo{0}
                , m_FifoIndex(0)
        {}

        std::tuple<std::array<T, fft_size>, std::array<T, fft_size>> calculate(juce::AudioBuffer<T>& x)
        {
            std::array<std::complex<float>, FFT_SIZE> in, out;
            for (int i = fft_size; --i >=0;) { in.at(i) = test_buffer.getSample(0, i); }

            fft.perform(in.data(), out.data(), false);

            std::array<T, fft_size> mag, phase;
            for (int i = fft_size; --i >=0;)
            {
                mag.at(i) = std::abs(out.at(i));
                phase.at(i) = std::abs(out.at(i)) <= std::numeric_limits<float>::epsilon() ? 0 : std::atan2(out.at(i).imag(), out.at(i).real());
            }

            auto argmax = meta::argmax(mag.begin(), mag.end());
            auto distance = std::distance(mag.begin(), argmax);

            return std::make_pair(mag, phase);
        }

    private:
        juce::dsp::FFT m_FFT;
        juce::dsp::WindowingFunction<T> m_Window;
        std::array<T, fft_size> m_Fifo;
        int m_FifoIndex;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MagPhaseCalculator);
    };
}
}
