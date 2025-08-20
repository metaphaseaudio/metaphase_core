/*
 * AllTests.cpp
 *      Author: Matt Zapp
 */
#include <gtest/gtest.h>
#include <inc/meta/testing/TestingJUCEContextHandler.h>

#include "test/tests/util/test_graphing_plot.h"
#include "test/tests/util/test_math.h"
#include "test/tests/util/test_Interpolate.h"
#include "test/tests/util/test_range.h"
#include "test/tests/util/test_simd_ops.h"
#include "test/tests/util/test_BitSize.h"
#include "test/tests/util/test_BitMask.h"
#include "test/tests/util/test_Countdown.h"
#include "test/tests/util/test_FixedPoint.h"
#include "test/tests/util/test_FixedPointMath.h"
#include "test/tests/util/container_helpers/test_TwoDimensionalHeap.h"
#include "test/tests/util/container_helpers/test_comparisons.h"
#include "test/tests/util/container_helpers/test_Vector.h"
#include "test/tests/util/container_helpers/test_Array.h"
#include "test/tests/util/container_helpers/test_zip.h"
#include "test/tests/generators/test_complementary_sequence.h"
#include "test/tests/generators/test_LoopingAccumulator.h"
#include "test/tests/generators/test_range_sequence.h"
#include "test/tests/audio/test_Asymp.h"
#include "test/tests/audio/test_decibels.h"
#include "test/tests/dsp/test_Decimate.h"
#include "test/tests/audio/test_BufferAccumulator.h"
#include "test/tests/audio/test_BufferBuffer.h"
#include "test/tests/dsp/test_MagPhaseCalculator.h"
#include "test/tests/dsp/test_LinearResample.h"
#include "test/tests/dsp/test_FindInSignal.h"
#include "test/tests/dsp/test_Wavetable.h"
#include "test/tests/dsp/test_BlipSynth.h"
#include "test/tests/dsp/test_OversampledBuffer.h"
#include "test/tests/dsp/test_MultiChanConvolve.h"
#include "test/tests/dsp/test_SharedMemoryBlock.h"
#include "test/tests/generators/test_LinearRamp.h"
#include "test/tests/generators/test_SineSweep.h"
#include "test/tests/util/test_StringHelpers.h"


int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    meta::TestingJUCEContextHandler juce_context_handler;
	const auto result = RUN_ALL_TESTS();
	return result;
}
