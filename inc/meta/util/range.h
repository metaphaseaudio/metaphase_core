//
// Created by Matt on 1/11/2021.
//

#pragma once

#include <juce_core/juce_core.h>
#include <meta/util/math.h>

namespace meta
{
    template <typename T1, typename T2, typename T3>
    inline T3 remap_range(const T1 out_start, const T1 out_end, const T2 in_start, const T2 in_end, const T3& x)
        { return out_start + (x - in_start) * (out_end - out_start) / (in_end - in_start); }
}