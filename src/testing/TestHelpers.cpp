//
// Created by mzapp on 4/21/18.
//

#include "meta/testing/TestHelpers.h"


const juce::File meta::TestHelpers::testFolder
        = juce::File::getSpecialLocation(juce::File::SpecialLocationType::tempDirectory)
                .getChildFile("test_data");

bool meta::TestHelpers::bufferIsEqual(juce::AudioBuffer<float>& x, std::initializer_list<std::initializer_list<float>> y)
{
    if (y.size() != x.getNumChannels()) return false;

    for (auto c = x.getNumChannels(); --c >= 0;)
    {
        auto yc = y.begin() + c;
        if (yc->size() != x.getNumSamples()) return false;

        for (auto s = x.getNumSamples(); --s >= 0;)
        {
            auto xs = x.getSample(c,s);
            auto ys = *(yc->begin() + s);
            if (xs != ys) return false;
        }
    }

    return true;
}
