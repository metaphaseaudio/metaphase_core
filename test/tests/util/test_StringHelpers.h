//
// Created by Matt on 4/22/2023.
//

#pragma once
#include <meta/util/StringHelpers.h>


TEST(StringHelpers, split)
{
    std::string to_split = "hello_world_1";
    auto split = meta::StringHelpers::split(to_split, "_");
    ASSERT_EQ(split[0], "hello");
    ASSERT_EQ(split[1], "world");
    ASSERT_EQ(split[2], "1");
}

TEST(StringHelpers, join)
{
    std::vector<std::string> to_join = { "hello", "world", "1" };
    auto joined = meta::StringHelpers::join(to_join, "_");
    ASSERT_EQ(joined, "hello_world_1");
}
