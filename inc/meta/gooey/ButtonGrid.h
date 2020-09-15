//
// Created by mzapp on 4/16/18.
//
#pragma once

#include <JuceHeader.h>
#include "ComponentGrid.h"


namespace meta
{
    template <typename ButtonType, size_t ROWS, size_t COLS>
    class ButtonGrid
        : public ComponentGrid<ButtonType, ROWS, COLS>
        , public juce::ChangeBroadcaster
        , juce::Button::Listener
    {
    public:
        ButtonGrid()
        {
            for(auto& btn : this->m_Children)
                { btn->addListener(this); }
        }

        ~ButtonGrid() override
            { for(auto& btn : this->m_Children) { btn->removeListener(this); }}

        virtual void gridButtonClicked(juce::Button*) {};

    protected:
        void buttonClicked(juce::Button* btn) override
        {
            gridButtonClicked(btn);
            sendChangeMessage();
        }
    };
}
