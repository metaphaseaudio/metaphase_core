//
// Created by Matt on 8/1/2021.
//

#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

#include <memory>


class TestingComponentVisualizer
    : juce::ChangeListener
{
public:
    explicit TestingComponentVisualizer(juce::Component* owned_component = nullptr)
        : is_closed(false)
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
            window->setVisible(true);
        });
    };

    void show()
    {
        while (!is_closed) {}
    }


private:
    void changeListenerCallback(juce::ChangeBroadcaster* source) override
    {
        if (source == window.get()) { window.reset(nullptr); }
        is_closed = true;
    }

    class CloseSignalingWindow
        : public juce::DocumentWindow
        , public juce::ChangeBroadcaster
    {
    public:
        explicit CloseSignalingWindow(juce::Component* to_own, std::string name, juce::Colour bg, int btns, bool add_to_desktop=true)
            : juce::DocumentWindow(name, bg, btns, add_to_desktop)
        {
            if (to_own) { setContentOwned(to_own, true); }
            setResizable(true, true);
        }

        void closeButtonPressed() override
        {
            setVisible(false);
            sendChangeMessage();
        }
    };

    std::unique_ptr<CloseSignalingWindow> window;
    std::atomic<bool> is_closed;
};


TEST(GraphingPlot, basic_window)
{
    auto viz = std::make_unique<TestingComponentVisualizer>();
    viz->show();
}