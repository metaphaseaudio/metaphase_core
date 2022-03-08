//
// Created by Matt on 3/7/2022.
//

#pragma once
#include <meta/audio/Asymp.h>

TEST(Asymp, asymp_has_ended)
{
    auto asymp = meta::Asymp(48000);
    asymp.setValue(1);
    asymp.setTarget(0);
    asymp.setTime(1, 48000);

    for (int i = 0; i < 48001; i++)
    {
        asymp.tick();
        ASSERT_FALSE(asymp.hasEnded());
    }

    asymp.tick();
    ASSERT_TRUE(asymp.hasEnded());
}