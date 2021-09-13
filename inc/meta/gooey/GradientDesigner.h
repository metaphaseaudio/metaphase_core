//
// Created by Matt on 9/9/2021.
//

#pragma once
#include <juce_gui_basics/juce_gui_basics.h>


namespace meta
{
    class GradientDesigner
        : public juce::Component
        , juce::ChangeListener
    {
    public:
        class Display
            : public juce::Component
        {
        public:
            explicit Display(const juce::ColourGradient& gradient);
            void paint(juce::Graphics &g) override;

        private:
            const juce::ColourGradient& r_Gradient;
        };

        //////////

        class ColourPoint
            : public juce::Component
            , public juce::ChangeBroadcaster
            , juce::ChangeListener
        {
        public:
            void mouseDown(const juce::MouseEvent& e) override;
            void mouseDrag(const juce::MouseEvent& e) override;
            void mouseUp(const juce::MouseEvent& event) override;
            ColourPoint() = default;
            explicit ColourPoint(juce::Colour colour);

            void resized() override;
            void paint(juce::Graphics& g) override;
            juce::Colour getPointColour() const { return m_Colour; }

            void changeListenerCallback(juce::ChangeBroadcaster *source) override;

        private:
            juce::ComponentDragger drag_context;
            juce::ComponentBoundsConstrainer m_Constraints;
            juce::Colour m_Colour;
        };

        //////////

        GradientDesigner();
        void resized() override;
        void changeListenerCallback(juce::ChangeBroadcaster *source) override;

    private:
        juce::ColourGradient m_Gradient;
        Display m_Display;
        juce::Component m_Track;
        std::vector<std::unique_ptr<ColourPoint>> m_Colours;
    };
}
