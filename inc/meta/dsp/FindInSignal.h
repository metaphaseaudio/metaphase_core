//
// Created by Matt on 8/1/2020.
//
#pragma once
#include <juce_dsp/juce_dsp.h>

namespace meta
{
namespace dsp
{
    template <typename SampleType>
    void prepare_convolution_from_reference_and_capture(
            juce::dsp::Convolution& conv,
            const juce::AudioBuffer<SampleType>& cap,
            const juce::AudioBuffer<SampleType>& ref)
    {
        // Prep reference stream
        juce::AudioBuffer<float> ref_cp(ref);
        ref_cp.reverse(0, ref_cp.getNumSamples());

        // Prep convolution
        juce::dsp::ProcessSpec spec = { 1, std::max(cap.getNumSamples(), ref_cp.getNumSamples()) * 2, 1 };
        conv.prepare(spec);
        conv.copyAndLoadImpulseResponseFromBuffer(ref_cp, 1, false, false, false, ref.getNumSamples());
    }

    template <typename SampleType>
    void one_shot_convolution(
            juce::dsp::Convolution& conv,
            const juce::AudioBuffer<SampleType>& cap,
            const juce::AudioBuffer<SampleType>& ref)
    {
        // Prep reference stream
        juce::AudioBuffer<float> ref_cp(ref);
        ref_cp.reverse(0, ref_cp.getNumSamples());

        // Prep convolution
        juce::dsp::ProcessSpec spec = { 1, std::max(cap.getNumSamples(), ref_cp.getNumSamples()) * 2, 1 };
        conv.prepare(spec);
        conv.copyAndLoadImpulseResponseFromBuffer(ref_cp, 1, false, false, false, ref.getNumSamples());
    }

    /**
     * Locates a signal x in another signal y.
     *
     * @tparam SampleType the sample type in the audio buffer
     * @param x The target signal
     * @param y the signal to search
     * @return
     */
    template <typename SampleType>
    std::pair<long, long> find_in_signal(const juce::dsp::Convolution& conv, const juce::AudioBuffer<SampleType>& x)
    {
        auto tmp = juce::AudioBuffer<float>(1, total_length);
        auto out = juce::AudioBuffer<float>(cap.getNumChannels(), total_length);
        out.clear();


        long start = 0;
        long end = 0;

        return std::make_pair(start, end);
    }
}
}