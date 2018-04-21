//
// Created by mzapp on 4/21/18.
//

#include "meta/util/testing/TestBase.h"
#include <meta/util/file/AudioFileHelpers.h>

void meta::TestBase::initializeTestFile(const juce::File &f)
{
    testFile = f;
    if (testFile.exists()) { testFile.deleteFile(); }
    testFile.create();
    m_Writer.reset(meta::AudioFileHelpers::createWriter(testFile, 48000, 2));
}
