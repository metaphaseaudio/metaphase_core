//
// Created by Mattb on 8/3/2022.
//

#pragma once
#include <juce_events/juce_events.h>

namespace meta
{
    template <typename ValueType>
    class ChangeBroadcastingProperty
        : public juce::ChangeBroadcaster
    {
    public:
        explicit ChangeBroadcastingProperty(const ValueType& value)
            : m_Value(value)
        {}

        ValueType load() const { return m_Value.load(); }

        ChangeBroadcastingProperty& operator=(const ValueType& other)
        {
            m_Value = other;
            sendChangeMessage();
            return *this;
        }

        explicit operator ValueType() { return m_Value.load(); }

    private:
        std::atomic<ValueType> m_Value;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ChangeBroadcastingProperty<ValueType>);
    };
}