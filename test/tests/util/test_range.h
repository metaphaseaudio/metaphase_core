//
// Created by Matt on 1/11/2021.
//

#pragma once
#include <meta/util/range.h>


TEST(MetaRange, remap_basic)
{
    ASSERT_EQ(meta::remap_range(0.0f, 1.0f, 0, 100, 50.0f), 0.5f);
}

TEST(MetaRange, remap_with_negative)
{
    ASSERT_EQ(meta::remap_range(-1.0f, 1.0f, 0, 100, 50.0f), 0.0f);
}