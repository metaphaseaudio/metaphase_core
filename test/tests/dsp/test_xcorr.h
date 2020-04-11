//
// created by mzapp 3/22/2020
//
#pragma once

#include <algorithm>
#include <JuceHeader.h>
#include <meta/dsp/xcorr.h>
#include <meta/util/simd_ops.h>
#include <meta/util/container_helpers/vector.h>

template <typename T>
T calculate_normalization_denominator(const T* x, size_t xn, const T* y, size_t yn)
{
	meta::simd<T>::pow(tmp_data, x, xn, 2);
	auto sum_x = meta::simd<T>::accumulate(tmp_data, x.getNumSamples());

	meta::simd<T>::pow(tmp_data, y_data, y.getNumSamples(), 2);
	auto sum_y = meta::simd<T>::accumulate(tmp_data, y.getNumSamples());

	return std::sqrt(sum_x * sum_y);
}

template <typename T>
T calculate_normalization_denominator(const juce::AudioBuffer<T> x, const juce::AudioBuffer<T>& y)
{
	juce::AudioBuffer<float> tmp(1, x.getNumSamples() + y.getNumSamples());
	auto tmp_data = tmp.getArrayOfWritePointers()[0];

	const T* x_data = x.getArrayOfReadPointers()[0];
	meta::simd<T>::pow(tmp_data, x_data, x.getNumSamples(), 2);
	auto sum_x = meta::simd<T>::accumulate(tmp_data, x.getNumSamples());

	const T* y_data = y.getArrayOfReadPointers()[0];
	meta::simd<T>::pow(tmp_data, y_data, y.getNumSamples(), 2);
	auto sum_y = meta::simd<T>::accumulate(tmp_data, y.getNumSamples());

	return std::sqrt(sum_x * sum_y);
}

template <typename T>
void xcorr_impulse(juce::AudioBuffer<T>& dst, juce::AudioBuffer<T>& cap, const juce::AudioBuffer<T>& ref)
{
	juce::dsp::ProcessSpec spec = { 1, std::max(cap.getNumSamples(), ref.getNumSamples()) * 2, 1 };
	juce::dsp::Convolution conv;
	conv.prepare(spec);

	juce::AudioBuffer<float> ref_cp(ref);
	ref_cp.reverse(0, ref_cp.getNumSamples());
	conv.copyAndLoadImpulseResponseFromBuffer(ref_cp, 1, false, false, false, ref.getNumSamples());

    // Convolve
	juce::dsp::AudioBlock<float> in_block(cap);
	juce::dsp::AudioBlock<float> out_block(dst);
	juce::dsp::ProcessContextNonReplacing<float> context(in_block, out_block);
	conv.process(context);

	// Normalize
	auto denom = calculate_normalization_denominator(ref, cap);
	meta::simd<float>::div(dst.getArrayOfWritePointers()[0], denom, dst.getNumSamples());

	// Slice
}

TEST(CrossCorrelationTest, xcorr_lenth_mismatch_2_to_5)
{
	std::array<float, 3> refarr = { 1, 0, 0 };
	std::array<float, 7> caparr = { 0, 0, 1, 0, .5, 0, 0 };
	std::array<float, 20> tmp;
	float* refp = refarr.data();
	float* capp = caparr.data();
	auto sizeref = sizeof(refarr) / sizeof(float);
	auto sizecap = sizeof(caparr) / sizeof(float);
	juce::AudioBuffer<float> ref(&refp , 1, refarr.size());
	juce::AudioBuffer<float> cap(&capp, 1, caparr.size());
	juce::AudioBuffer<float> out(1, sizeref + sizecap - 1);

	xcorr_impulse(out, cap, ref);
}

