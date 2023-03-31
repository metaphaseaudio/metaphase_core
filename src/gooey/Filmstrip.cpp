//
// Created by Matt on 3/19/2023.
//

#include "meta/gooey/Filmstrip.h"

meta::Filmstrip::Filmstrip(juce::Image* image, int sliceSizePx, meta::Orientation sliceOrientation)
    : p_Img(image)
    , m_SliceSizePx(sliceSizePx)
    , m_Orient(sliceOrientation)
{
    jassert(image != nullptr); // The image has to exist.
    jassert(sliceSizePx >= 1);  // Slices must be at least 1px wide
}

juce::Image meta::Filmstrip::getFrame(int index) const
{
    // This will return an empty image so , but the caller should
    jassert(index * m_SliceSizePx + m_SliceSizePx <= (m_Orient == meta::Orientation::HORIZ ? p_Img->getWidth() : p_Img->getHeight()));

    auto bounds = juce::Rectangle<int>(
        m_Orient == meta::Orientation::HORIZ ? m_SliceSizePx * index : 0,
        m_Orient == meta::Orientation::HORIZ ? 0 : m_SliceSizePx * index,
        m_Orient == meta::Orientation::HORIZ ? m_SliceSizePx : p_Img->getWidth(),
        m_Orient == meta::Orientation::HORIZ ? p_Img->getHeight() : m_SliceSizePx
    );

    return p_Img->getClippedImage(bounds);
}

int meta::Filmstrip::getNFrames() const
{
    return ((m_Orient == HORIZ ? p_Img->getWidth() : p_Img->getHeight()) / m_SliceSizePx) - 1;
}
