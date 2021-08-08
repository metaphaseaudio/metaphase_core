//
// Created by Matt on 8/2/2021.
//

#include <juce_events/juce_events.h>
#include "inc/meta/testing/TestingJUCEContextHandler.h"


meta::TestingJUCEContextHandler::TestingJUCEContextHandler()
{
    message_thread = std::make_unique<std::thread>([](){
        juce::MessageManager::getInstance()->setCurrentThreadAsMessageThread();
        juce::MessageManager::getInstance()->runDispatchLoop();
        juce::DeletedAtShutdown::deleteAll();
    });
}


meta::TestingJUCEContextHandler::~TestingJUCEContextHandler()
{
    juce::MessageManager::getInstance()->stopDispatchLoop();
    message_thread->join();
    juce::MessageManager::deleteInstance();
}

