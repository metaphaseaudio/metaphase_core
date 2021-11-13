//
// Created by Matt on 9/23/2021.
//

#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

namespace meta
{
class RulerComponent
    : public juce::Component
{
public:
    enum Orientation
    {
        Vertical = 0,
        Horizontal,
        Square
    };

    RulerComponent(juce::Range<float> range);

    void paint(juce::Graphics &g) override;
    void resized() override;

    Orientation getOrientation() const;
private:
    int m_MajorMarks, m_MinorMarks;
    juce::Range<float> m_Range;
};
}


