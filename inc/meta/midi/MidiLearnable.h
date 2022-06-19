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
            virtual void startLearn(MidiLearnBroadcaster* learnable) = 0;
            virtual void unlearn(MidiLearnBroadcaster* learnable) = 0;
        };

        MidiLearnBroadcaster() = default;
        virtual ~MidiLearnBroadcaster() = default;

        void addMidiLearnListener(Listener* const listener);
        void removeMidiLearnListener(Listener* const listener);

        void setLearnedControl(juce::MidiMessage ctrl) { m_Ctrl = ctrl; }

        bool isLearned() const;
        [[ nodiscard ]] const juce::MidiMessage& getLearnedControl() const { return m_Ctrl; }

        void sendLearn();
        void sendUnlearn();
        void removeAllListeners();

        virtual void handleMidiMessage(const juce::MidiMessage& msg) = 0;

    protected:
        juce::MidiMessage m_Ctrl;
    private:
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
