//
// Created by mzapp on 4/21/18.
//

#include "meta/testing/TestHelpers.h"


const juce::File meta::TestHelpers::testFolder
        = juce::File::getSpecialLocation(juce::File::SpecialLocationType::tempDirectory)
                .getChildFile("test_data");
