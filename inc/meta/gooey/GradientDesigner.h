//
// Created by Matt on 9/9/2021.
//

#pragma once
#include <juce_gui_basics/juce_gui_basics.h>


namespace meta
{
    class GradientDesigner
        : public juce::Component
        , public juce::ChangeBroadcaster
        , juce::ChangeListener
    {
    public:
        class Display
            : public juce::Component
        {
        public:
            explicit Display(const juce::ColourGradient& gradient);
            void paint(juce::Graphics &g) override;
            const juce::ColourGradient& gradient;
        };

        ///////////////////////////////////////////////////////////////////////
        class ColourPoint
            : public juce::Component
            , public juce::ChangeBroadcaster
            , juce::ChangeListener
        {
        public:
            ColourPoint() = default;
            explicit ColourPoint(juce::Colour colour, float proportion);

            void resized() override;
            void paint(juce::Graphics& g) override;

            void mouseDown(const juce::MouseEvent& e) override;
            void mouseDrag(const juce::MouseEvent& e) override;
            void mouseUp(const juce::MouseEvent& event) override;

            void pickColour();
            juce::Colour getPointColour() const { return m_Colour; }
            float getProportionAlongGradient() const { return m_Proportion; }
            void changeListenerCallback(juce::ChangeBroadcaster *source) override;
        private:
            juce::ComponentDragger drag_context;
            juce::ComponentBoundsConstrainer m_Constraints;
            juce::Colour m_Colour;
            float m_Proportion;
        };

        ///////////////////////////////////////////////////////////////////////
        GradientDesigner();
        void setGradient(const juce::ColourGradient& gradient);
        juce::ColourGradient getGradient() const { return m_Gradient; };

        void resized() override;
        void mouseDown(const juce::MouseEvent &event) override;
        void changeListenerCallback(juce::ChangeBroadcaster *source) override;

    private:
        void addPoint(const juce::Colour& colour, float position);
        void removePoint(ColourPoint* toRemove);
        void refreshGradient();
        const int icon_size;
        juce::ColourGradient m_Gradient;
        Display m_Display;
        juce::Component m_Track;
        std::vector<std::unique_ptr<ColourPoint>> m_Colours;
    };
}
