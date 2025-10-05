//
// Created by Matt on 3/19/2023.
//

#include <utility>

#include "meta/gooey/Filmstrip.h"

meta::Filmstrip::Filmstrip(juce::Image image, int frameCount, meta::Orientation sliceOrientation)
    : m_Img(std::move(image))
    , m_FrameCount(frameCount)
    , m_Orient(sliceOrientation)
    , m_SliceSizePx((sliceOrientation == meta::HORIZ ? m_Img.getWidth() : m_Img.getHeight()) / frameCount)
{
    jassert(frameCount >= 1);  // There has to be at least one frame
}

meta::Filmstrip& meta::Filmstrip::operator=(const Filmstrip& other)
{
    if (this == &other)
        return *this;
    m_Img = other.m_Img;
    m_FrameCount = other.m_FrameCount;
    m_SliceSizePx = other.m_SliceSizePx;
    m_Orient = other.m_Orient;
    return *this;
}

meta::Filmstrip& meta::Filmstrip::operator=(Filmstrip&& other) noexcept
{
    if (this == &other)
        return *this;
    m_Img = std::move(other.m_Img);
    m_FrameCount = other.m_FrameCount;
    m_SliceSizePx = other.m_SliceSizePx;
    m_Orient = other.m_Orient;
    return *this;
}

meta::Filmstrip::Filmstrip(meta::Filmstrip&& other) noexcept
    : m_Img(other.m_Img)
    , m_FrameCount(other.m_FrameCount)
    , m_Orient(other.m_Orient)
    , m_SliceSizePx(other.m_SliceSizePx)
{}

juce::Image meta::Filmstrip::getFrame(int index) const
{
    // This will return an empty image so, whatever, but the caller should know.
    jassert(index * m_SliceSizePx + m_SliceSizePx <= (m_Orient == meta::Orientation::HORIZ ? m_Img.getWidth() : m_Img.getHeight()));

    auto bounds = juce::Rectangle<int>(
        m_Orient == meta::Orientation::HORIZ ? m_SliceSizePx * index : 0,
        m_Orient == meta::Orientation::HORIZ ? 0 : m_SliceSizePx * index,
        m_Orient == meta::Orientation::HORIZ ? m_SliceSizePx : m_Img.getWidth(),
        m_Orient == meta::Orientation::HORIZ ? m_Img.getHeight() : m_SliceSizePx
    );

    return m_Img.getClippedImage(bounds);
}


juce::Rectangle<int> meta::Filmstrip::getFrameDimensions() const
{
    return m_Orient == HORIZ
                    ? juce::Rectangle<int>(m_SliceSizePx, m_Img.getHeight())
                    : juce::Rectangle<int>(m_Img.getWidth(), m_SliceSizePx);
}
