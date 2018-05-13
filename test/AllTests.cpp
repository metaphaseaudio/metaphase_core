/*
 * AllTests.cpp
 *      Author: Matt Zapp
 */
#include <gtest/gtest.h>
#include "test/tests/util/test_BitSize.h"
#include "test/tests/util/test_BitMask.h"
#include "test/tests/util/test_FixedPoint.h"
#include "test/tests/util/test_FixedPointMath.h"

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
