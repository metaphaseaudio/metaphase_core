//
// Created by Matt on 1/11/2021.
//

#pragma once

#include <juce_core/juce_core.h>

namespace meta
{
    template <typename T>
    T remap_range(const juce::Range<T>& out_range, const juce::Range<T>& in_range, const T& x)
    {
        return out_range.getStart() + (x - in_range.getStart()) *
               (out_range.getEnd() - out_range.getStart()) / (in_range.getEnd() - in_range.getStart());
    }
}