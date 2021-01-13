//
// Created by mzapp on 4/21/18.
//
#pragma once

#ifdef META_BUILD_TESTS
#include <gtest/gtest.h>
#include <juce_audio_formats/juce_audio_formats.h>

namespace meta
{
    class TestBase
        : public testing::Test
    {
    protected:

        void initializeTestFile(const juce::File &f);

        void TearDown() override;

        std::unique_ptr<juce::AudioFormatWriter> m_Writer;
        juce::File testFile;
    };
}
#endif