//
// Created by Matt on 8/2/2021.
//

#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include <memory>


namespace meta
{
    class TestingComponentVisualizer
            : juce::ChangeListener
    {
    public:
        explicit TestingComponentVisualizer(juce::Component* owned_component = nullptr);
        void show();

    private:
        void changeListenerCallback(juce::ChangeBroadcaster* source) override;

        class CloseSignalingWindow
            : public juce::DocumentWindow
            , public juce::ChangeBroadcaster
        {
        public:
            CloseSignalingWindow(juce::Component* to_own, std::string name, juce::Colour bg, int btns, bool add_to_desktop=true);
            void closeButtonPressed() override;
        };

        std::unique_ptr<CloseSignalingWindow> window;
        std::atomic<bool> is_closed;
    };
}
