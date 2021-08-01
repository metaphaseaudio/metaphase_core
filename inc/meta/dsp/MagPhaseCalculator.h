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
            , m_Window{0}
            , m_Fifo{0}
            , m_FifoIndex(0)
        {
            m_Window.fill(1);
            juce::dsp::WindowingFunction<T> wind(fft_size, juce::dsp::WindowingFunction<T>::hann);
            wind.multiplyWithWindowingTable(m_Window.data(), fft_size);
        }

        std::tuple<std::array<T, fft_size>, std::array<T, fft_size>> calculate(const juce::AudioBuffer<T>& x)
        {
            std::array<std::complex<float>, fft_size> in{0};
            std::array<std::complex<float>, fft_size> out{0};
            for (int i = std::min(x.getNumSamples(), fft_size); --i >=0;) { in.at(i) = x.getSample(0, i) * m_Window.at(i); }
            m_FFT.perform(in.data(), out.data(), false);

            std::array<T, fft_size> mag, phase;
            for (int i = fft_size; --i >=0;)
            {
                mag.at(i) = std::abs(out.at(i));
                phase.at(i) = std::abs(out.at(i)) <= std::numeric_limits<float>::epsilon() ? 0 : std::atan2(out.at(i).imag(), out.at(i).real());
            }

            return std::make_pair(mag, phase);
        }

    private:
        juce::dsp::FFT m_FFT;
        std::array<T, fft_size> m_Fifo, m_Window;
        int m_FifoIndex;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MagPhaseCalculator);
    };
}
}
