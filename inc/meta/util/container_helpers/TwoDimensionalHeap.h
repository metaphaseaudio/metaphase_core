//
// Created by Matt on 8/16/2021.
//

#pragma once
#include <juce_core/juce_core.h>


namespace meta
{
    template <typename T>
    class TwoDimensionalHeap
        : public juce::HeapBlock<T>
    {
    public:
        TwoDimensionalHeap(int nX, int nY)
            : m_X(nX)
            , m_Y(nY)
        {
            resize(m_X, m_Y);
        };

        void resize(int nX, int nY)
        {
            m_X = nX;
            m_Y = nY;
            m_2D.resize(m_Y);
            juce::HeapBlock<T>::realloc(sizeof(T) * size());
            for (int y = m_Y; --y >=0;) { m_2D.at(y) = this->get() + (y * m_X); }
        }

        T** twoD() { return m_2D.data(); };
        int size() { return m_X * m_Y; };
        int sizeX() { return m_X; }
        int sizeY() { return m_Y; }

    private:
        int m_X, m_Y;
        std::vector<T*> m_2D;
    };
}

