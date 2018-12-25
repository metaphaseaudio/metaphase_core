//
// Created by mzapp on 12/24/18.
//
#pragma once

#include "ButtonGrid.h"

namespace meta
{
    template <typename ButtonType, size_t ROWS, size_t COLS>
    class RadioGrid
        : public ButtonGrid<ButtonType, ROWS, COLS>
    {
    public:
        RadioGrid() { this->m_Buttons.at(0).setToggleState(true, juce::dontSendNotification); }

    private:
        void buttonClicked(juce::Button* btn) override
        {
            for (auto& b : this->m_Buttons) { b.setToggleState(false, juce::dontSendNotification); }
            btn->setToggleState(true, juce::dontSendNotification);
            this->gridButtonClicked(btn);
            this->sendChangeMessage();
        }
    };
}
