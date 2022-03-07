//
// Created by Matt on 3/6/2022.
//

#pragma once


#include <meta/audio/LoopingAccumulator.h>


TEST(LoopingAccumulatorTest, one_tick)
{
    auto accum = meta::LoopingAccumulator<0, 100>();
    accum.tick();
}