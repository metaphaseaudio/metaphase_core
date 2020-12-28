//
// Created by Matt on 9/20/2020.
//

#pragma once
#include <JuceHeader.h>


namespace meta
{
    class TimedParameterListener
        : juce::AudioProcessorParameter::Listener
        , juce::Timer
    {
    public:
        explicit TimedParameterListener(juce::AudioProcessorParameter& param)
            : r_Parameter (param)
        {
            r_Parameter.addListener (this);
            startTimer (100);
        }

        ~TimedParameterListener() override { r_Parameter.removeListener (this); }

        juce::AudioProcessorParameter& getParameter() noexcept { return r_Parameter; }

        virtual void handleNewParameterValue() = 0;

    private:
        //==============================================================================
        void parameterValueChanged (int, float) override { parameterValueHasChanged = true; }
        void parameterGestureChanged (int, bool) override {}

        //==============================================================================
        void timerCallback() override
        {
            if (parameterValueHasChanged)
            {
                handleNewParameterValue();
                startTimerHz(50);
            }
            else
            {
                // backoff refresh
                startTimer(std::min(250, getTimerInterval() + 10));
            }
        }

        std::atomic_bool parameterValueHasChanged;
        juce::AudioProcessorParameter& r_Parameter;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TimedParameterListener)
    };
}

