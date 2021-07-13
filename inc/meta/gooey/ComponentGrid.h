//
// Created by Matt on 9/13/2020.
//

#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "meta/util/container_helpers/array.h"


namespace meta
{
    template <typename ComponentType, size_t ROWS, size_t COLS, size_t MARGIN=5>
    class ComponentGrid
        : public juce::Component
    {
    public:
        constexpr static unsigned int BUTTON_COUNT = {ROWS * COLS};

        template <typename... Args>
        explicit ComponentGrid(Args&&... args)
        {
            for (int i = 0; i < BUTTON_COUNT; i++)
            {
                m_Children.emplace_back(std::make_unique<ComponentType>(args...));
                addAndMakeVisible(this->m_Children.at(i).get());
            }
        }

        void resized() override
        {
            const auto bounds = getLocalBounds();
            const auto height = (getHeight() - (MARGIN * (ROWS - 1))) / static_cast<float>(ROWS);
            const auto width = (getHeight() - (MARGIN * (COLS - 1))) / static_cast<float>(COLS);
            const juce::Rectangle<float> btnDimensions(width, height);

            for (int row = 0; row < ROWS; row++)
            {
                const auto firstID = COLS * row;
                const auto rowTop = (row * MARGIN) + (row * height);

                for (int col = 0; col < COLS; col++)
                {
                    const auto colLeft = (col * MARGIN) + (col * width);
                    const auto childID = firstID + col;
                    auto& child = m_Children.at(childID);
                    child->Component::setBounds(btnDimensions.toNearestInt());
                    child->Component::setTopLeftPosition(int(colLeft), int(rowTop));
                }
            }
        }

    protected:
        std::vector<std::unique_ptr<ComponentType>> m_Children;
    };
}