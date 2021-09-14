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
//
//        class PointTrack
//            : public juce::Component
//            , public juce::ChangeBroadcaster
//        {
//            void mouseDoubleClick(const juce::MouseEvent &event) override;
//        };

        ///////////////////////////////////////////////////////////////////////
        class ColourPoint
            : public juce::Component
            , public juce::ChangeBroadcaster
            , juce::ChangeListener
        {
        public:
            void pickColour();
            void mouseDown(const juce::MouseEvent& e) override;
            void mouseDrag(const juce::MouseEvent& e) override;
            void mouseUp(const juce::MouseEvent& event) override;
            ColourPoint() = default;
            explicit ColourPoint(juce::Colour colour);

            void resized() override;
            void paint(juce::Graphics& g) override;
            juce::Colour getPointColour() const { return m_Colour; }
            float getColourPosition() const;

            void changeListenerCallback(juce::ChangeBroadcaster *source) override;

        private:
            juce::ComponentDragger drag_context;
            juce::ComponentBoundsConstrainer m_Constraints;
            juce::Colour m_Colour;
        };

        ///////////////////////////////////////////////////////////////////////
        GradientDesigner();
        void refreshGradient();
        void resized() override;
        void mouseDown(const juce::MouseEvent &event) override;
        void changeListenerCallback(juce::ChangeBroadcaster *source) override;

    private:
        const int icon_size;
        juce::ColourGradient m_Gradient;
        Display m_Display;
        juce::Component m_Track;
        std::vector<std::unique_ptr<ColourPoint>> m_Colours;
    };
}
