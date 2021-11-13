//
// Created by Matt on 9/23/2021.
//

#include <meta/gooey/RulerComponent.h>

meta::RulerComponent::Orientation meta::RulerComponent::getOrientation() const
{
    const auto bounds = getLocalBounds();
    if      (bounds.getWidth() == bounds.getHeight()) { return Orientation::Square; }
    else if (bounds.getWidth() >  bounds.getHeight()) { return Orientation::Horizontal; }
    return meta::RulerComponent::Vertical;
}

meta::RulerComponent::RulerComponent(juce::Range<float> range)
    : m_MajorMarks(5)
    , m_MinorMarks(5)
{}

void meta::RulerComponent::paint(juce::Graphics& g)
{
    g.setColour(juce::Colours::black);
    g.fillAll();

    if (getOrientation() == Orientation::Square) { return; }

    auto vert = getOrientation() == Orientation::Vertical;
    auto localBounds = getLocalBounds().toFloat();
    auto length = vert ? localBounds.getHeight() : localBounds.getWidth();
    auto majorDistance = length / float(m_MajorMarks - 1);
    auto minorDistance = majorDistance / float(m_MinorMarks + 1);

    for (auto major_i = 0; major_i < m_MajorMarks; major_i++)
    {
        float position = majorDistance * major_i;
        g.setColour(juce::Colours::white);
        if (vert) { g.drawHorizontalLine(position, 0, getWidth()); }
        else      { g.drawVerticalLine(position, 0, getHeight()); }

        g.setColour(juce::Colours::grey);
        for (auto minor_i = 0; minor_i < m_MinorMarks; minor_i++)
        {

            if (vert) { g.drawHorizontalLine(position + (minorDistance * (minor_i + 1)), 0, getWidth()); }
            else      { g.drawVerticalLine(position + (minorDistance * (minor_i + 1)), 0, getHeight()); }
        }
    }
}

void meta::RulerComponent::resized() {}
