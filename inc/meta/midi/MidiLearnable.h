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

        void addMidiLearnListener(Listener* const listener);
        void removeMidiLearnListener(Listener* const listener);

        void setLearnedCtrlNumber(int ctrl) { m_CtrlNumber = ctrl; }
        int getLearnedCtrlNumber() const { return m_CtrlNumber; }

        void sendLearn();
        void sendUnlearn();
        void removeAllListeners();

        virtual void handleMidiMessage(const juce::MidiMessage& msg) = 0;

    private:

        int m_CtrlNumber;
        juce::ListenerList<Listener> m_LearnListeners;

        JUCE_DECLARE_NON_COPYABLE(MidiLearnBroadcaster);
    };

    //-------------------------------------------------------------------------
    class MidiLearnableAudioParameterFloat
        : public MidiLearnBroadcaster
        , public juce::AudioParameterFloat
    {
    public:
        using juce::AudioParameterFloat::AudioParameterFloat;

        MidiLearnableAudioParameterFloat& operator=(float);

        void handleMidiMessage(const juce::MidiMessage& msg) override;
    };
}
