//
// Created by Matt on 8/8/2021.
//

#pragma once
#include <juce_gui_basics/juce_gui_basics.h>


juce::ApplicationCommandManager& getCommandManager();
juce::ApplicationProperties& getAppProperties();
bool isOnTouchDevice();
