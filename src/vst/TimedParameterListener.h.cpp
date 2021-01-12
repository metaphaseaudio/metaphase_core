//
// Created by Matt on 1/11/2021.
//

#include "meta/vst/TimedParameterListener.h"

void meta::TimedParameterListener::timerCallback()
{
    auto t = true;
    if (m_value_changed.compare_exchange_strong(t, false, std::memory_order_release, std::memory_order_relaxed))
    {
        auto b = m_value_changed.load();
        std::cout << b;
        handleNewParameterValue();
        startTimerHz(50);
    }
    else
    {
        // backoff refresh
        startTimer(std::min(250, getTimerInterval() + 10));
    }
}