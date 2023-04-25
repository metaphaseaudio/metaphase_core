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
        Filmstrip(juce::Image image, int frameCount, meta::Orientation sliceOrientation);
        Filmstrip (Filmstrip&&) noexcept;

        [[nodiscard]] int getNFrames() const { return m_FrameCount; };
        [[nodiscard]] juce::Rectangle<int> getFrameDimensions() const;
        [[nodiscard]] juce::Image getFrame(int index) const;

    private:
        const juce::Image m_Img;
        int m_FrameCount, m_SliceSizePx;
        meta::Orientation m_Orient;
    };
}
