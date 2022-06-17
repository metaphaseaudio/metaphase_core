//
// Created by Mattb on 6/13/2022.
//

#include "meta/midi/MidiLearnable.h"

void meta::MidiLearnBroadcaster::sendLearn()
    { m_LearnListeners.call([this](Listener& l) { l.learn(this); }); }

void meta::MidiLearnBroadcaster::sendUnlearn()
    { m_LearnListeners.call([this](Listener& l) { l.unlearn(this); }); }

void meta::MidiLearnBroadcaster::addMidiLearnListener(meta::MidiLearnBroadcaster::Listener* const listener)
    { m_LearnListeners.add(listener); }


void meta::MidiLearnBroadcaster::removeMidiLearnListener(meta::MidiLearnBroadcaster::Listener* const listener)
    { m_LearnListeners.remove(listener); }


void meta::MidiLearnBroadcaster::removeAllListeners()
    { m_LearnListeners.clear(); }

//=============================================================================

void meta::MidiLearnableAudioParameterFloat::handleMidiMessage(const juce::MidiMessage& msg)
{
    if (m_Ctrl.getControllerNumber() == msg.getControllerNumber() && m_Ctrl.getChannel() == msg.getChannel())
    {
        setValueNotifyingHost(float(msg.getControllerValue()) / 127.0f);
    }
}

meta::MidiLearnableAudioParameterFloat& meta::MidiLearnableAudioParameterFloat::operator=(float value)
{
    juce::AudioParameterFloat::operator=(value);
    return *this;
}
