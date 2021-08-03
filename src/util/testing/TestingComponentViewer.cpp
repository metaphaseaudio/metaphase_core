//
// Created by Matt on 8/2/2021.
//

#include "inc/meta/util/testing/TestingComponentViewer.h"

meta::TestingComponentVisualizer::TestingComponentVisualizer(juce::Component* owned_component)
    : is_closed(true)
{
    juce::MessageManager::getInstance()->callAsync([this, owned_component](){
        window = std::make_unique<CloseSignalingWindow>(owned_component, "New Window", juce::Colours::black, juce::DocumentWindow::allButtons);
        window->setUsingNativeTitleBar(true);
        window->setCentrePosition(400, 400);

        if (owned_component != nullptr)
        {
        window->setContentOwned(owned_component, true);
        }
        else
        {
        window->setSize(200, 200);
        }

        window->setResizable(false, false);
        window->addChangeListener(this);
    });
};


void meta::TestingComponentVisualizer::show()
{
    is_closed = false;
    juce::MessageManager::getInstance()->callAsync([this](){ window->setVisible(true); });
    while (!is_closed) {}
}


void meta::TestingComponentVisualizer::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    if (source == window.get()) { window.reset(nullptr); }
    is_closed = true;
}


meta::TestingComponentVisualizer::CloseSignalingWindow::CloseSignalingWindow
(juce::Component* to_own, std::string name, juce::Colour bg, int btns, bool add_to_desktop)
    : juce::DocumentWindow(name, bg, btns, add_to_desktop)
{
    if (to_own) { setContentOwned(to_own, true); }
    setResizable(true, true);
}


void meta::TestingComponentVisualizer::CloseSignalingWindow::closeButtonPressed()
{
    setVisible(false);
    sendChangeMessage();
}
