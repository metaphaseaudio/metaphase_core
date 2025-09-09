//
// Created by mzapp on 4/21/18.
//

#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_core/juce_core.h>


namespace meta
{
    class TestHelpers
    {
    public:
        static const juce::File testFolder;
        static bool bufferIsEqual(juce::AudioBuffer<float>& x, std::initializer_list<std::initializer_list<float>>);
    };
}

