//
// Created by Matt on 8/2/2021.
//

#include <juce_events/juce_events.h>
#include "inc/meta/util/testing/TestingJUCEMessageThreadHandler.h"


meta::TestingJUCEMessageThreadHandler::TestingJUCEMessageThreadHandler()
{
    message_thread = std::make_unique<std::thread>([](){
        juce::MessageManager::getInstance()->setCurrentThreadAsMessageThread();
        juce::MessageManager::getInstance()->runDispatchLoop();
        juce::DeletedAtShutdown::deleteAll();
    });
}


meta::TestingJUCEMessageThreadHandler::~TestingJUCEMessageThreadHandler()
{
    juce::MessageManager::getInstance()->stopDispatchLoop();
    message_thread->join();
    juce::MessageManager::deleteInstance();
}

