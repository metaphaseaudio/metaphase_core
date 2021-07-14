//
// Created by Matt on 8/1/2020.
//
#pragma once
#include <meta/util/container_helpers/comparisons.h>
#include <meta/dsp/MultiChanConvolve.h>

namespace meta
{
namespace dsp
{

    /**
     * Locates a convolution-prepared signal x in another signal y.
     *
     * @tparam SampleType the sample type in the audio buffer
     * @param x The target signal
     * @param y the signal to search
     * @return
     */
    std::pair<long, long> find_in_signal(meta::dsp::MultiChanConvolve& conv, const juce::AudioBuffer<float>& y)
    {
        juce::AudioBuffer<float> convolved = conv.convolve(y);
        juce::FloatVectorOperations::abs(convolved.getWritePointer(0), convolved.getReadPointer(0), convolved.getNumSamples());
        auto start = convolved.getArrayOfReadPointers()[0];
        auto ptr_max = argmax(start, start + convolved.getNumSamples());
        const auto found_end = ptr_max - start;
        return std::make_pair(found_end - conv.get_ir_length() + 1, found_end);
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
        juce::AudioBuffer<SampleType> cpy(x);
        cpy.reverse(0, cpy.getNumSamples());
        meta::dsp::MultiChanConvolve conv(std::move(cpy), 1024);
        return find_in_signal(conv, y);
    }
}
}