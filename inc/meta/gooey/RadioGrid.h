//
// Created by mzapp on 12/24/18.
//
#pragma once

#include "ButtonGrid.h"

namespace meta
{
    template <typename ButtonType, size_t ROWS, size_t COLS, size_t MARGIN=5>
    class RadioGrid
        : public ButtonGrid<ButtonType, ROWS, COLS, MARGIN>
    {
    public:
        RadioGrid() { this->m_Children.at(0)->setToggleState(true, juce::dontSendNotification); }

    private:
        void buttonClicked(juce::Button* btn) override
        {
            for (auto& b : this->m_Children) { b->setToggleState(false, juce::dontSendNotification); }
            btn->setToggleState(true, juce::dontSendNotification);
            this->gridButtonClicked(btn);
            this->sendChangeMessage();
        }
    };
}
