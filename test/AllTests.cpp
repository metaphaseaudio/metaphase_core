/*
 * AllTests.cpp
 *      Author: Matt Zapp
 */
#include <gtest/gtest.h>

#include "test/tests/util/test_math.h"
#include "test/tests/util/test_range.h"
#include "test/tests/util/test_simd_ops.h"
#include "test/tests/util/test_BitSize.h"
#include "test/tests/util/test_BitMask.h"
#include "test/tests/util/test_FixedPoint.h"
#include "test/tests/util/test_FixedPointMath.h"
#include "test/tests/util/container_helpers/test_comparisons.h"
#include "test/tests/util/container_helpers/test_Vector.h"
#include "test/tests/util/container_helpers/test_Array.h"
#include "test/tests/util/container_helpers/test_zip.h"
#include "test/tests/generators/test_complementary_sequence.h"
//#include "test/tests/dsp/test_FindInSignal.h"
#include "test/tests/dsp/test_WavetableOscillator.h"
#include "test/tests/dsp/test_MultiChanConvolve.h"
#include "test/tests/generators/test_LinearRamp.h"
#include "test/tests/generators/test_SineSweep.h"


int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
	const auto result = RUN_ALL_TESTS();
	return result;
}
