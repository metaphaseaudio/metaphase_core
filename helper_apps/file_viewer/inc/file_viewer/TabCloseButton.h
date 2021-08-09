//
// Created by Matt on 8/9/2021.
//

#pragma once
#include <juce_gui_basics/juce_gui_basics.h>


class TabCloseButton
    : public juce::Button
{
public:
    explicit TabCloseButton(const juce::String& name) : juce::Button(name) { setSize(20, 20); };
    void paintButton(juce::Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TabCloseButton);
};


