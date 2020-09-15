//
// Created by Matt on 9/12/2020.
//
#pragma once
#include <JuceHeader.h>
#include <meta/midi/MidiState.h>


namespace meta
{
    class MidiComponent
        : public juce::Component
        , public MidiState::Listener
    {
    public:
        explicit MidiComponent(meta::MidiState& keyboardState);
        ~MidiComponent() override;

    protected:
        virtual juce::MidiMessage getMidiMessageForMouseEvent(const juce::MouseEvent& e, bool down) = 0;
        MidiState& r_MidiState;

    private:
        /** @internal */
        void mouseDown(const juce:: MouseEvent&) override;
        /** @internal */
        void mouseUp (const juce::MouseEvent&) override;
    };
}