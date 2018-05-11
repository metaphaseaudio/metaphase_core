//
// Created by mzapp on 4/10/18.
//

#pragma once
#include <meta/util/BitMask.h>

TEST(BitMaskHelpersTest, mask_bits)
{
    ASSERT_EQ(1, (meta::BitMaskHelpers<uint32_t, 0>::maskBits()));
    ASSERT_EQ(3, (meta::BitMaskHelpers<uint32_t, 1>::maskBits()));
    ASSERT_EQ(7, (meta::BitMaskHelpers<uint32_t, 2>::maskBits()));
}

TEST(BitMaskTest, mask_bottom_16)
{
    ASSERT_EQ(0xFFFF, meta::BitMask<uint32_t>::Right<16>());
}

TEST(BitMaskTest, mask_top_16)
{
    ASSERT_EQ(0xFFFF0000, meta::BitMask<uint32_t>::Left<16>());
}

TEST(BitMaskTest, sign)
{
    ASSERT_EQ(0,  meta::BitMask<int32_t>::Sign(300));
    ASSERT_EQ(-0, meta::BitMask<int32_t>::Sign(-20));
}