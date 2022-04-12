//
// Created by Matt on 1/11/2021.
//

#pragma once

#include <juce_core/juce_core.h>
#include <meta/util/math.h>

namespace meta
{
    template <typename T1, typename T2, typename T3>
    T3 remap_range(const juce::Range<T1>& out_range, const juce::Range<T2>& in_range, const T3& x)
    {
        return out_range.getStart() + (x - in_range.getStart()) *
               (out_range.getEnd() - out_range.getStart()) / (in_range.getEnd() - in_range.getStart());
    }
}