//
// Created by mzapp on 4/21/18.
//

#include "meta/util/testing/TestBase.h"
#include <meta/util/file/AudioFileHelpers.h>

#ifdef META_BUILD_TESTS
void meta::TestBase::initializeTestFile(const juce::File &f, int sampleRate, int chans)
{
    testFile = f;
    if (testFile.exists()) { testFile.deleteFile(); }
    testFile.create();
    m_Writer.reset(meta::AudioFileHelpers::createWriter(testFile, sampleRate, chans));
}

void meta::TestBase::TearDown()
{
    if (m_Writer != nullptr)
    {
        m_Writer->flush();
        m_Writer.reset(nullptr);
    }
}
#endif
