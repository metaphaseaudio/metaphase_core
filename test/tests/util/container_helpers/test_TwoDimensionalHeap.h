//
// Created by Matt on 8/17/2021.
//

#pragma once
#include <meta/util/container_helpers/TwoDimensionalHeap.h>


TEST(TwoDHeap, test_int)
{
    auto heap = meta::TwoDimensionalHeap<int>(2, 4);
    for (int i = heap.size(); --i >=0; ) { heap.get()[i] = i; }
    ASSERT_EQ(heap.twoD()[0][1], 1);
}