//
// Created by Matt on 9/13/2020.
//
#include "meta/midi/MidiState.h"


void meta::MidiState::enqueueMessage(const juce::MidiMessage& msg)
{
    const std::unique_lock<std::mutex> lock(m_Mutex);

    const int timeNow = (int) juce::Time::getMillisecondCounter();
    m_ToAdd.addEvent (msg, timeNow);
    m_ToAdd.clear (0, timeNow - 500);
}

void meta::MidiState::processNextMidiEvent(const juce::MidiMessage& message)
    { for (auto listener : m_Listeners) { listener->handleMessage(message); } }

void meta::MidiState::processNextMidiBuffer(juce::MidiBuffer& buffer, const int startSample, const int numSamples, const bool injectIndirectEvents)
{
    juce::MidiBuffer::Iterator i (buffer);
    juce::MidiMessage message;
    int time;

    const std::unique_lock<std::mutex> lock(m_Mutex);

    while (i.getNextEvent(message, time)) { processNextMidiEvent (message);}

    if (injectIndirectEvents)
    {
        juce::MidiBuffer::Iterator i2 (m_ToAdd);
        const int firstEventToAdd = m_ToAdd.getFirstEventTime();
        const double scaleFactor = numSamples / (double) (m_ToAdd.getLastEventTime() + 1 - firstEventToAdd);

        while (i2.getNextEvent (message, time))
        {
            const int pos = juce::jlimit (0, numSamples - 1, juce::roundToInt ((time - firstEventToAdd) * scaleFactor));
            buffer.addEvent (message, startSample + pos);
        }
    }

    m_ToAdd.clear();
}


void meta::MidiState::addListener(meta::MidiState::Listener* listener)
{
    m_Listeners.push_back(listener);
}

void meta::MidiState::removeListener(meta::MidiState::Listener* listener)
{
    m_Listeners.erase(std::find(m_Listeners.begin(), m_Listeners.end(), listener));
}

