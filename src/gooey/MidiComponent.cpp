//
// Created by Matt on 9/12/2020.
//
#include "meta/gooey/MidiComponent.h"

meta::MidiComponent::MidiComponent(meta::MidiState &keyboardState)
    : r_MidiState(keyboardState)
{ r_MidiState.addListener(this); }

meta::MidiComponent::~MidiComponent() { r_MidiState.removeListener(this); }

void meta::MidiComponent::mouseDown(const juce::MouseEvent &e)
{
    r_MidiState.enqueueMessage(getMidiMessageForMouseEvent(e, true));
}

void meta::MidiComponent::mouseUp(const juce::MouseEvent &e)
{
    r_MidiState.enqueueMessage(getMidiMessageForMouseEvent(e, false));
}

