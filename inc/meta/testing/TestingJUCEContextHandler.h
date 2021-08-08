//
// Created by Matt on 8/2/2021.
//

#pragma once
#include <juce_core/juce_core.h>

namespace meta
{
    class TestingJUCEContextHandler
    {
    public:
        TestingJUCEContextHandler();
        ~TestingJUCEContextHandler();
    private:
        std::unique_ptr<std::thread> message_thread;
    };
}
