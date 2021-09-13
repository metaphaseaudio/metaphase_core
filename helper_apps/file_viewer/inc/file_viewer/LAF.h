//
// Created by Matt on 8/9/2021.
//

#pragma once
#include <inc/meta/gooey/MetaLookAndFeel.h>

class LAF
    : public meta::MetaLookAndFeel
{
public:
    LAF();

    int getTabButtonBestWidth(juce::TabBarButton& button, int tabDepth) override;
};


