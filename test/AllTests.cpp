/*
 * AllTests.cpp
 *      Author: Matt Zapp
 */
#include <gtest/gtest.h>

#include "test/tests/util/test_math.h"
#include "test/tests/util/test_vector_math.h"
#include "test/tests/util/test_BitSize.h"
#include "test/tests/util/test_BitMask.h"
#include "test/tests/util/test_FixedPoint.h"
#include "test/tests/util/test_FixedPointMath.h"
#include "test/tests/util/container_helpers/test_Vector.h"
#include "test/tests/util/container_helpers/test_Array.h"
#include "test/tests/generators/test_Golay.h"
#include "test/tests/dsp/test_WavetableOscillator.h"
#include "test/tests/dsp/test_xcorr.h"
#include "test/tests/generators/test_LinearRamp.h"


int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
	const auto result = RUN_ALL_TESTS();
	return result;
}
