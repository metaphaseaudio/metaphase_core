//
// created by mzapp 3/22/2020
//
#pragma once

#include <algorithm>
#include <JuceHeader.h>
#include <meta/dsp/xcorr.h>
#include <meta/util/container_helpers/vector.h>



TEST(CrossCorrelationTest, xcorr_5_samps)
{
	std::vector<float> x{ 1, 0, 0, 0, 0 };
	std::vector<float> y{ 1, 2, 3, 4, 5 };
	std::vector<float> test{ 0, 0, 0, 0, 1, 2, 3, 4, 5 };
	auto xcorr = meta::xcorr(x, y);
	EXPECT_EQ(xcorr, test);
}


TEST(CrossCorrelationTest, xcorr_lenth_mismatch_2_to_5)
{
	juce::dsp::ProcessSpec spec = {1, 100, 1};
	float refarr[] = { 1, 0, 0 };
	float caparr[] = { 0, 0, 1, 2, .5, 0, 0 };
	float power[20];
	float* refp = refarr;
	float* capp = caparr;
	auto sizeref = sizeof(refarr) / sizeof(float);
	auto sizecap = sizeof(caparr) / sizeof(float);

	juce::AudioBuffer<float> ref(&refp , 1, sizeref);
	juce::AudioBuffer<float> cap(&capp, 1, sizecap);
	juce::AudioBuffer<float> outb(1, sizeref + sizecap - 1);
	juce::dsp::AudioBlock<float> in_block(cap);
	juce::dsp::AudioBlock<float> out_block(outb);

	juce::dsp::ProcessContextNonReplacing<float> context(in_block, out_block);

	juce::FloatVectorOperations::multiply(capp, capp, sizecap);


	juce::dsp::Convolution conv;
	conv.prepare(spec);

	ref.reverse(0, ref.getNumSamples());
	conv.copyAndLoadImpulseResponseFromBuffer(ref, 1, false, false, false, sizeref);
	conv.process(context);
}


template<typename T>
std::vector<T>
conv(std::vector<T> const &f, std::vector<T> const &g) {
	int const nf = f.size();
	int const ng = g.size();
	int const n = nf + ng - 1;
	std::vector<T> out(n, T());
	for (auto i(0); i < n; ++i) {
		int const jmn = (i >= ng - 1) ? i - (ng - 1) : 0;
		int const jmx = (i < nf - 1) ? i : nf - 1;
		for (auto j(jmn); j <= jmx; ++j) {
			out[i] += (f[j] * g[i - j]);
		}
	}
	return out;
}


TEST(VectorConvolution, vector_convolution)
{
	std::vector<float> ref{ 1, 0, 0 };
	std::vector<float> cap{ 0, 0, 0, 1, 0, 5, 0 };
	std::reverse(ref.begin(), ref.end());
	auto out = conv(cap, ref);	
	auto impulse = std::get<1>(meta::split(out, ref.size() - 1));
	auto reconvolved = conv(ref, impulse);
	auto trimmed = std::get<1>(meta::split(reconvolved, ref.size()));

	ASSERT_EQ(reconvolved, trimmed);
}
