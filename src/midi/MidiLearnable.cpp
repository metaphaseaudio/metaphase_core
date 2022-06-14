//
// Created by Mattb on 6/13/2022.
//

#include "meta/midi/MidiLearnable.h"

void meta::MidiLearnBroadcaster::sendLearn()
    { m_LearnListeners.call([this](Listener& l) { l.learn(this); }); }

void meta::MidiLearnBroadcaster::sendUnlearn()
    { m_LearnListeners.call([this](Listener& l) { l.unlearn(this); }); }

void meta::MidiLearnBroadcaster::addListener(meta::MidiLearnBroadcaster::Listener* const listener)
    { m_LearnListeners.add(listener); }

void meta::MidiLearnBroadcaster::removeAllListeners()
    { m_LearnListeners.clear(); }

void meta::MidiLearnBroadcaster::removeListener(meta::MidiLearnBroadcaster::Listener* const listener)
    { m_LearnListeners.remove(listener); }
