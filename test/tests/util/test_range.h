//
// Created by Matt on 1/11/2021.
//

#pragma once
#include <meta/util/range.h>


TEST(MetaRange, remap_basic)
{
    auto out_range = juce::Range<float>(0.0f, 1.0f);
    auto in_range = juce::Range<float>(0, 100);
    ASSERT_EQ(meta::remap_range(out_range, in_range, 50.0f), 0.5f);
}

TEST(MetaRange, remap_with_negative)
{
    auto out_range = juce::Range<float>(-1.0f, 1.0f);
    auto in_range = juce::Range<float>(0, 100);
    ASSERT_EQ(meta::remap_range(out_range, in_range, 50.0f), 0.0f);
}