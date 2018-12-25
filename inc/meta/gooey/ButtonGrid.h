//
// Created by mzapp on 4/16/18.
//
#pragma once

#include <JuceHeader.h>

namespace meta
{
    template <typename ButtonType, size_t ROWS, size_t COLS>
    class ButtonGrid
        : public juce::Component
        , public juce::ChangeBroadcaster
        , juce::Button::Listener
    {
    public:
        constexpr static unsigned int BUTTON_COUNT = {ROWS * COLS};

        ButtonGrid()
        {
            for(auto& btn : m_Buttons)
            {
                btn.addListener(this);
                addAndMakeVisible(btn);
            }
        }

        virtual ~ButtonGrid()
            { for(auto& btn : m_Buttons) { btn.removeListener(this); }}

        void resized() override
        {
            const auto sequencerBounds = getLocalBounds().reduced(2);
            const float btnMargin = 5;

            const auto btnHeight = (getHeight() / static_cast<float>(ROWS)) - btnMargin;
            const auto btnWidth = (getWidth() / static_cast<float>(COLS)) - btnMargin;
            const juce::Rectangle<float> btnDimensions(btnWidth, btnHeight);

            for (int row = 0; row < ROWS; row++)
            {
                const auto firstID = COLS * row;
                const auto rowTop = ((row + 1) * btnMargin) + (row * btnHeight);

                for (int col = 0; col < COLS; col++)
                {
                    const auto colLeft = ((col + 1) * btnMargin) + (col * btnWidth);
                    const auto btnID = firstID + col;
                    auto& btn = m_Buttons.at(btnID);
                    btn.setBounds(btnDimensions.toNearestInt());
                    btn.setTopLeftPosition(colLeft, rowTop);
                }
            }
        }

        virtual void gridButtonClicked(juce::Button*) = 0;

    protected:
        virtual void buttonClicked(juce::Button* btn) override
        {
            gridButtonClicked(btn);
            sendChangeMessage();
        }

        std::array<ButtonType, BUTTON_COUNT> m_Buttons;
    };
}
