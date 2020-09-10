//
// Created by Matt on 6/29/2020.
//

#pragma once

namespace meta
{
    class SelectableComponent
    {
    public:
        explicit SelectableComponent(bool initial_state)
            : m_Selected(initial_state)
        {};

        bool isSelected() const { return m_Selected; }
        void setSelected(bool x) { m_Selected = x; }

    private:
        bool m_Selected
    };
}