//
// Created by Matt on 8/1/2020.
//
#pragma once
#include <JuceHeader.h>
#include <meta/util/container_helpers/comparisons.h>


namespace meta
{
namespace dsp
{
    template <typename SampleType>
    void prepare_convolution(
            const juce::AudioBuffer<SampleType>& impulse,
            juce::dsp::Convolution& to_prepare,
            const int max_block_size=0)
    {
        // Prep reference stream  We assume IRs need to be reversed.
        juce::AudioBuffer<float> imp_cp(impulse);
        imp_cp.reverse(0, imp_cp.getNumSamples());

        // Prep convolution
        juce::dsp::ProcessSpec spec = { 1, std::max(impulse.getNumSamples(), max_block_size), 1 };
        to_prepare.prepare(spec);
        to_prepare.copyAndLoadImpulseResponseFromBuffer(imp_cp, 1, false, false, false, impulse.getNumSamples());
    }


    /**
     * Locates a convolution-prepared signal x in another signal y.
     *
     * @tparam SampleType the sample type in the audio buffer
     * @param x The target signal
     * @param y the signal to search
     * @return
     */
    template <typename SampleType>
    std::pair<long, long> find_in_signal(const juce::dsp::Convolution& conv, const juce::AudioBuffer<SampleType>& y)
    {
//        juce::dsp::AudioBlock<float> in_block(cap.getArrayOfWritePointers() + chan, 1, 0, cap.getNumSamples());
//        juce::dsp::AudioBlock<float> out_block(tmp);
//        juce::dsp::ProcessContextNonReplacing<float> context(in_block, out_block);
//        conv.process(context);
//        auto context = juce::dsp::ProcessContextNonReplacing<SampleType>();
//        conv.process();
//        auto max = argmax()
        return std::make_pair(0,0);
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
    std::pair<long, long> find_in_signal(const juce::AudioBuffer<SampleType>& x, const juce::AudioBuffer<SampleType>& y)
    {

        return std::make_pair(0,0);
    }
}
}