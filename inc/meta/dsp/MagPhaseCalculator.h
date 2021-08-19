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

        explicit MagPhaseCalculator(int fft_order)
            : m_FFT(fft_order)
            , fft_size(std::pow(2, fft_order))
            , m_Window(fft_size)
        {
            std::fill(m_Window.begin(), m_Window.end(), 1);
            juce::dsp::WindowingFunction<T> wind(fft_size, juce::dsp::WindowingFunction<T>::hann);
            wind.multiplyWithWindowingTable(m_Window.data(), fft_size);
        }

        MagPhaseFrame calculate_window(juce::AudioBuffer<T>& x, int chan, int start_sample) const
            { return calculate_window(juce::dsp::AudioBlock<T>(x), chan, start_sample); }

        MagPhaseFrame calculate_window(const juce::dsp::AudioBlock<T>& x, int chan, int start_sample) const
        {
            std::vector<T> mag(fft_size), phase(fft_size);
            calculate_window(x, mag.data(), phase.data(), chan, start_sample);
            return std::make_pair(mag, phase);
        }

        void calculate_window(const juce::dsp::AudioBlock<T>& x, T* mag_out, T* phase_out, int chan, int start_sample) const
        {
            jassert(chan <= x.getNumChannels() && chan >= 0);
            jassert(start_sample <= x.getNumSamples() && start_sample >=0);
            calculate_window_unchecked(x.getChannelPointer(chan) + start_sample, x.getNumSamples() - start_sample, mag_out, phase_out);
        }

        void calculate_window_unchecked(const T* x, int n_samples, T* mag_out, T* phase_out) const
            { convert_results(run_fft(x, n_samples), mag_out, phase_out); }

    private:
        std::vector<std::complex<T>> run_fft(const T* data, int n_samples) const
        {
            std::vector<std::complex<T>> in(fft_size);
            std::vector<std::complex<T>> out(fft_size);

            // copy * window into complex type for fft. only calculate a window or less.
            for (auto i = std::min<int>(n_samples, fft_size); --i >=0;) { in.at(i) = *(data + i) * m_Window.at(i); }

            m_FFT.perform(in.data(), out.data(), false);

            return out;
        }

        void convert_results(const std::vector<std::complex<T>>& in, T* mag_out, T* phase_out) const
        {
            for (int i = fft_size; --i >=0;)
            {
                const auto mag = std::abs(in.at(i));
                const auto phase = mag <= std::numeric_limits<float>::epsilon() ? 0 : std::atan2(in.at(i).imag(), in.at(i).real());
                *(mag_out + i) = mag;
                *(phase_out + i) = phase;
            }
        };

        juce::dsp::FFT m_FFT;
        std::vector<T> m_Window;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MagPhaseCalculator);
    };
}
}
