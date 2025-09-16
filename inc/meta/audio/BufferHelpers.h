//
// Created by Matt on 9/16/2025.
//

#pragma once

namespace meta
{
    template <typename FloatType>
    FloatType channel_power(juce::AudioBuffer<FloatType>& x, int c)
    {
        FloatType rv = 0;
        for (int i = x.getNumSamples(); --i >= 0;)
        {
            rv += std::pow(std::abs(x.getSample(c, i)), 2);
        }
        return rv;
    }
}
