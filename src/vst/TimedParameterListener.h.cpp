//
// Created by Matt on 1/11/2021.
//

#include "meta/vst/TimedParameterListener.h"

void meta::TimedParameterListener::timerCallback()
{
    auto t = true;
    if (m_value_changed.compare_exchange_weak(t, false, std::memory_order_release, std::memory_order_relaxed))
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