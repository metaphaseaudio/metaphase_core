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
        explicit TimedParameterListener(juce::AudioProcessorParameter* param)
            : m_value_changed(param != nullptr)
            , p_Parameter(param)
        {
            if (param == nullptr) { return; }
            p_Parameter->addListener (this);
            startTimer (100);
        }

        ~TimedParameterListener() override
        {
            if (p_Parameter == nullptr) { return; }
            p_Parameter->removeListener (this);
        }


        virtual void handleNewParameterValue() = 0;

    protected:
        juce::AudioProcessorParameter* p_Parameter;

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

