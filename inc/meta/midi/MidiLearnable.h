//
// Created by Mattb on 6/13/2022.
//

#pragma once
#include <juce_audio_processors/juce_audio_processors.h>

namespace meta
{
    class MidiLearnBroadcaster
    {
    public:
        class Listener
        {
        public:
            virtual void learn(MidiLearnBroadcaster* learnable) = 0;
            virtual void unlearn(MidiLearnBroadcaster* learnable) = 0;
        };

        MidiLearnBroadcaster() = default;
        virtual ~MidiLearnBroadcaster() = default;

        void addListener(Listener* const listener);
        void removeListener(Listener* const listener);

        void sendLearn();
        void sendUnlearn();
        void removeAllListeners();

        virtual void handleMidiMessage(const juce::MidiMessage& msg) = 0;

    private:
        juce::ListenerList<Listener> m_LearnListeners;

        JUCE_DECLARE_NON_COPYABLE(MidiLearnBroadcaster);
    };
}
