//
// Created by mzapp on 4/10/18.
//

#pragma once
#include <meta/util/BitSize.h>

TEST(BitSizeTest, get_bit_size_8)  { ASSERT_EQ(8,  meta::WordSizeInBits<uint8_t>::Value()); }
TEST(BitSizeTest, get_bit_size_16) { ASSERT_EQ(16, meta::WordSizeInBits<uint16_t>::Value()); }
TEST(BitSizeTest, get_bit_size_32) { ASSERT_EQ(32, meta::WordSizeInBits<uint32_t>::Value()); }
TEST(BitSizeTest, get_bit_size_64) { ASSERT_EQ(64, meta::WordSizeInBits<uint64_t>::Value()); }