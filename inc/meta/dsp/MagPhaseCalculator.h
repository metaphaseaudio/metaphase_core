//
// Created by Matt on 7/25/2021.
//

#pragma once
#include <juce_dsp/juce_dsp.h>
#include <meta/util/math.h>

namespace meta
{
namespace dsp
{
    template <typename T>
    class MagPhaseCalculator
    {
    public:
        const size_t fft_size;
        using FFTFrame = std::vector<T>;
        using MagPhaseFrame = std::pair<FFTFrame, FFTFrame>;

        MagPhaseCalculator(size_t fft_order)
            : m_FFT(fft_order)
            , fft_size(std::pow(2, fft_order))
            , m_Window(fft_size)
        {
            std::fill(m_Window.begin(), m_Window.end(), 1);
            juce::dsp::WindowingFunction<T> wind(fft_size, juce::dsp::WindowingFunction<T>::hann);
            wind.multiplyWithWindowingTable(m_Window.data(), fft_size);
        }


        MagPhaseFrame calculate_window(juce::AudioBuffer<T>& x, int chan, int start_sample)
        {
            juce::dsp::AudioBlock<T> y(x);
            return calculate_window(y, chan, start_sample);
        }


        MagPhaseFrame calculate_window(const juce::dsp::AudioBlock<T>& x, int chan, int start_sample)
        {
            jassert(chan <= x.getNumChannels() && chan >= 0);
            jassert(start_sample <= x.getNumSamples() && start_sample >=0);

            std::vector<std::complex<T>> in(fft_size);
            std::vector<std::complex<T>> out(fft_size);

            for (int i = std::min<int>(x.getNumSamples() - start_sample, fft_size); --i >=0;)
                { in.at(i) = x.getSample(chan, i + start_sample) * m_Window.at(i); }

            m_FFT.perform(in.data(), out.data(), false);

            std::vector<T> mag, phase;
            for (int i = fft_size; --i >=0;)
            {
                mag.push_back(std::abs(out.at(i)));
                phase.push_back(std::abs(out.at(i)) <= std::numeric_limits<float>::epsilon() ? 0 : std::atan2(out.at(i).imag(), out.at(i).real()));
            }

            return std::make_pair(mag, phase);
        }

    private:
        juce::dsp::FFT m_FFT;
        std::vector<T> m_Window;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MagPhaseCalculator);
    };
}
}
