//
// Created by mzapp on 11/22/18.
//
#pragma once

#include <JuceHeader.h>
#include <deque>
#include "meta/util/enum/Orientation.h"

namespace meta
{
    template <typename BtnType, size_t BtnCount>
    class RadioButtonSet
        : public juce::Component
        , public juce::ChangeBroadcaster
        , juce::Button::Listener
    {
    public:
        /**
         * Creates a set of radio buttons
         * @param buttonNames
         */
        RadioButtonSet(const std::array<std::string, BtnCount>& buttonNames
                     , const Orientation& orient)
            : m_Selection(0)
            , m_Orientation(orient)
        {
            for (int i = BtnCount; --i >= 0;)
            {
                m_Btns.at(i).setName(buttonNames[i]);
                m_Btns.at(i).addListener(this);
                addAndMakeVisible(m_Btns.at(i));
            }
        }

        void setOrientation(const Orientation& orientation)
        {
            m_Orientation = orientation;
            resized();
        }

        void resized() override
        {
            auto margin = 2;
            auto bounds = getLocalBounds().reduced(margin);
            auto dimension = ((m_Orientation == Orientation::VERT
                                             ? bounds.getHeight()
                                             : bounds.getWidth()) / BtnCount) - margin;
            for (auto& btn : m_Btns)
            {
                auto rect = m_Orientation == Orientation::VERT
                                          ? bounds.removeFromTop(dimension)
                                          : bounds.removeFromLeft(dimension);
                btn.setBounds(rect);
                if (m_Orientation == Orientation::VERT) { bounds.removeFromTop(margin); }
                else                                    { bounds.removeFromLeft(margin); }
            }
        }

        void buttonClicked(juce::Button* clicked) override
        {
            for (int i = m_Btns.size(); --i >= 0;)
            {
                auto& btn = m_Btns.at(i);
                if (&btn == clicked) { m_Selection = i; }
                btn.setToggleState(false, juce::NotificationType::dontSendNotification);
            }

            clicked->setToggleState(true, juce::NotificationType::dontSendNotification);
            sendChangeMessage();
        }

        size_t getSelection() const { return m_Selection; }

    private:
        std::array<BtnType, BtnCount> m_Btns;
        size_t m_Selection;
        Orientation m_Orientation;
    };
}
