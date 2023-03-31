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
        Filmstrip(juce::Image* image, int sliceSizePx, meta::Orientation sliceOrientation);
        int getNFrames() const;
        [[nodiscard]] juce::Image getFrame(int index) const;

    private:
        const juce::Image* p_Img;
        int m_SliceSizePx;
        meta::Orientation m_Orient;
    };
}
