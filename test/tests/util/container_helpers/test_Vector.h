//
// Created by Matt Zapp on 3/22/2020.
//

#pragma once
#include <meta/util/container_helpers/vector.h>


TEST(VectorSliceTest, test_slice_first_3)
{
	std::vector<int> test { 1, 2, 3 };
	std::vector<int> x { 1, 2, 3, 4, 5 };
	auto sliced = meta::slice(x, 0, 3);
	EXPECT_EQ(sliced, test);
}


TEST(VectorSliceTest, test_slice_middle_3)
{
	std::vector<int> test{ 2, 3, 4 };
	std::vector<int> x{ 1, 2, 3, 4, 5 };
	auto sliced = meta::slice(x, 1, 4);
	EXPECT_EQ(sliced, test);
}


TEST(VectorSliceTest, test_slice_last_3)
{
	std::vector<int> test{ 3, 4, 5 };
	std::vector<int> x{ 1, 2, 3, 4, 5 };
	auto sliced = meta::slice(x, 2, x.size());
	EXPECT_EQ(sliced, test);
}


TEST(VectorSliceTest, test_slice_reverse_first_3)
{
	std::vector<int> test{ 3, 2, 1 };
	std::vector<int> x{ 1, 2, 3, 4, 5 };
	auto sliced = meta::slice(x, 3, 0);
	EXPECT_EQ(sliced, test);
}


TEST(VectorSplitTest, test_split_none)
{
	std::vector<int> test;
	std::vector<int> remainder{ 1, 2, 3, 4, 5 };
	std::vector<int> x{ 1, 2, 3, 4, 5 };
	auto sliced = meta::split(x, 0);
	EXPECT_EQ(std::get<0>(sliced), test);
	EXPECT_EQ(std::get<1>(sliced), remainder);
}


TEST(VectorSplitTest, test_split_first)
{
	std::vector<int> test{ 1 };
	std::vector<int> remainder{ 2, 3, 4, 5 };
	std::vector<int> x{ 1, 2, 3, 4, 5 };
	auto sliced = meta::split(x, 1);
	EXPECT_EQ(std::get<0>(sliced), test);
	EXPECT_EQ(std::get<1>(sliced), remainder);
}


TEST(VectorSplitTest, test_split_half)
{
	std::vector<int> test{ 1, 2, 3 };
	std::vector<int> remainder{ 4, 5 };
	std::vector<int> x{ 1, 2, 3, 4, 5 };
	auto sliced = meta::split(x, 3);
	EXPECT_EQ(std::get<0>(sliced), test);
	EXPECT_EQ(std::get<1>(sliced), remainder);
}


TEST(VectorSplitTest, test_split_all)
{
	std::vector<int> test{ 1, 2, 3, 4, 5 };
	std::vector<int> remainder;
	std::vector<int> x{ 1, 2, 3, 4, 5 };
	auto sliced = meta::split(x, 5);
	EXPECT_EQ(std::get<0>(sliced), test);
	EXPECT_EQ(std::get<1>(sliced), remainder);
}
