//
// Created by Matt on 11/14/2021.
//

#pragma once
#include <meta/audio/decibels.h>

TEST(MetaAudioDecibels, gain_to_dbfs)
{
    ASSERT_NEAR(meta::gain_coeff_to_db(0.5), -6.02, 0.001);
}