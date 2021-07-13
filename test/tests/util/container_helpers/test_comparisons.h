//
// Created by Matt on 7/7/2021.
//

#pragma once
#include <meta/util/container_helpers/comparisons.h>


TEST(ArgmaxTest, test_get_argmax_iterator)
{
    std::vector<int> x { 1, 2, 3, 4, 5, 4, 3, 2, 1};
    auto arg_max = meta::argmax(x.begin(), x.end());
    EXPECT_EQ(4, std::distance(x.begin(), arg_max));
    EXPECT_EQ(5, *arg_max);
}


TEST(ArgmaxTest, test_get_argmax_pointer)
{
    int x[9] = { 1, 2, 3, 4, 5, 4, 3, 2, 1};
    auto arg_max = meta::argmax(x, 9);
    EXPECT_EQ(4, arg_max - x);
    EXPECT_EQ(5, *arg_max);
}