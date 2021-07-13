//
// Created by Matt on 9/20/2020.
//

#pragma once
#include <juce_audio_processors/juce_audio_processors.h>


namespace meta
{
    class TimedParameterListener
        : protected juce::AudioProcessorParameter::Listener
        , juce::Timer
    {
    public:
        explicit TimedParameterListener(juce::AudioProcessorParameter& param)
            : m_value_changed(true)
            , r_Parameter(param)
        {
            r_Parameter.addListener (this);
            startTimer (100);
        }

        ~TimedParameterListener() override { r_Parameter.removeListener (this); }

        juce::AudioProcessorParameter& getParameter() noexcept { return r_Parameter; }

        virtual void handleNewParameterValue() = 0;

    protected:
        juce::AudioProcessorParameter& r_Parameter;

    private:
        //==============================================================================
        void parameterValueChanged (int, float) override { m_value_changed = true; }
        void parameterGestureChanged (int, bool) override {}

        //==============================================================================
        void timerCallback() override;

        std::atomic<bool> m_value_changed;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TimedParameterListener)
    };
}

