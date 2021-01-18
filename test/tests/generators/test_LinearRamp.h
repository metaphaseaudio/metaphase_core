//
// Created by Matt Zapp on 3/15/2020.
//

#pragma once

#include <meta/util/testing/TestBase.h>
#include <meta/util/testing/TestHelpers.h>
#include <meta/util/file/AudioFileHelpers.h>
#include <meta/generators/LinearRamp.h>

class LinearRampTest
	: public meta::TestBase
{
public:
	LinearRampTest()
		: m_Ramp(0, 1, 2)
	{};

	void initializeTestFile(const juce::String& f)
	{
		meta::TestBase::initializeTestFile(meta::TestHelpers::testFolder.getChildFile(f));
	}

	meta::LinearRamp<float> m_Ramp;
};


TEST_F(LinearRampTest, one_tick)
{
	EXPECT_EQ(m_Ramp.getProgress(), 0);
	EXPECT_EQ(m_Ramp.tick(), 0);
    EXPECT_EQ(m_Ramp.tick(), 0.5);
	EXPECT_EQ(m_Ramp.tick(), 1);
	EXPECT_EQ(m_Ramp.getProgress(), 1);
}

TEST_F(LinearRampTest, set_upside_down_ramp)
{
	m_Ramp.setStart(1);
	m_Ramp.setEnd(0);
	m_Ramp.setTicks(4);
	m_Ramp.reset();
	EXPECT_EQ(m_Ramp.tick(), 1);
	EXPECT_EQ(m_Ramp.tick(), .75);
	EXPECT_EQ(m_Ramp.tick(), .5);
	EXPECT_EQ(m_Ramp.tick(), .25);
	EXPECT_EQ(m_Ramp.tick(), 0);
}