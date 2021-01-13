//
// Created by Matt on 9/13/2020.
//

#pragma once
#include <mutex>
#include <juce_audio_basics/juce_audio_basics.h>


namespace meta
{
    class MidiState
    {
    public:
        class Listener
        {
        public:
            virtual void handleMessage(const juce::MidiMessage& msg) = 0;
        };

        void addListener(Listener* listener);
        void removeListener(Listener* listener);

        void enqueueMessage(const juce::MidiMessage& msg);

        void processNextMidiEvent(const juce::MidiMessage& message);
        void processNextMidiBuffer(juce:: MidiBuffer& buffer, int startSample, int numSamples, bool injectIndirectEvents);

    private:
        std::mutex m_Mutex;
        juce::MidiBuffer m_ToAdd;
        std::vector<Listener*> m_Listeners;
    };
}