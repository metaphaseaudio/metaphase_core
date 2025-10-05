//
// Created by Matt on 3/19/2023.
//

#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include "meta/util/enum/Orientation.h"

namespace meta
{
    class Filmstrip
    {
    public:
        Filmstrip(juce::Image image, int frameCount, Orientation sliceOrientation);
        Filmstrip(const Filmstrip& other) = default;
        Filmstrip (Filmstrip&& other) noexcept;
        Filmstrip& operator=(const Filmstrip& other);
        Filmstrip& operator=(Filmstrip&& other) noexcept;

        [[nodiscard]] int getNFrames() const { return m_FrameCount; };
        [[nodiscard]] juce::Rectangle<int> getFrameDimensions() const;
        [[nodiscard]] juce::Image getFrame(int index) const;

    private:
        juce::Image m_Img;
        int m_FrameCount, m_SliceSizePx;
        Orientation m_Orient;
    };
}
