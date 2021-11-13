//
// Created by Matt on 9/9/2021.
//

#include <meta/gooey/GradientDesigner.h>
#include <meta/gooey/MetaLookAndFeel.h>
#include <juce_gui_extra/juce_gui_extra.h>
#include <inc/meta/util/container_helpers/array.h>

meta::GradientDesigner::Display::Display(const juce::ColourGradient& gradient)
    : gradient(gradient)
{}

void meta::GradientDesigner::Display::paint(juce::Graphics& g)
    { dynamic_cast<meta::MetaLookAndFeel*>(&getLookAndFeel())->drawGradientDesignerDisplay(g, *this); }

///////////////////////////////////////////////////////////////////////////////
meta::GradientDesigner::ColourPoint::ColourPoint(juce::Colour colour, float proportion)
    : m_Colour(colour)
    , m_Proportion(proportion)
{}

void meta::GradientDesigner::ColourPoint::paint(juce::Graphics& g)
    { dynamic_cast<meta::MetaLookAndFeel*>(&getLookAndFeel())->drawGradientDesignerColourPoint(g, *this); }

void meta::GradientDesigner::ColourPoint::resized()
    { m_Constraints.setMinimumOnscreenAmounts(getHeight(), getWidth(), getHeight(), getWidth()); }

void meta::GradientDesigner::ColourPoint::mouseDown(const juce::MouseEvent& e)
{
    auto clickCount = e.getNumberOfClicks();
    if (clickCount > 1) { pickColour(); }
    else if (e.mods.isCtrlDown())
    {
        const auto designer = dynamic_cast<GradientDesigner*>(getParentComponent()->getParentComponent());
        designer->removePoint(this);
        designer->refreshGradient();
    }
    else { drag_context.startDraggingComponent (this, e); }
}

void meta::GradientDesigner::ColourPoint::mouseDrag(const juce::MouseEvent& e)
    { drag_context.dragComponent (this, e, &m_Constraints); }

void meta::GradientDesigner::ColourPoint::mouseUp(const juce::MouseEvent& event)
{
    const float x = getPosition().x;
    m_Proportion = x / getParentWidth();
    sendChangeMessage();
}

void meta::GradientDesigner::ColourPoint::pickColour()
{
    auto colourSelector = std::make_unique<juce::ColourSelector>
            (juce::ColourSelector::showColourAtTop | juce::ColourSelector::showSliders | juce::ColourSelector::showColourspace);
    colourSelector->setSize(300, 300);
    colourSelector->setCurrentColour(m_Colour, juce::dontSendNotification);
    colourSelector->addChangeListener(this);

    auto& box = juce::CallOutBox::launchAsynchronously(std::move(colourSelector), getScreenBounds(), nullptr);
}

void meta::GradientDesigner::ColourPoint::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    auto callout = dynamic_cast<juce::ColourSelector*>(source);
    m_Colour = callout->getCurrentColour();
    sendChangeMessage();
    repaint();
}

///////////////////////////////////////////////////////////////////////////////
meta::GradientDesigner::GradientDesigner()
    : m_Display(m_Gradient)
    , icon_size(15)
{
    m_Track.addMouseListener(this, false);

    addAndMakeVisible(m_Display);
    addAndMakeVisible(m_Track);

    // Setup default gradient
    m_Gradient.isRadial = false;
    m_Colours.emplace_back(std::make_unique<ColourPoint>(juce::Colours::yellow, 0));
    m_Colours.emplace_back(std::make_unique<ColourPoint>(juce::Colours::blue, 1));
    m_Gradient.addColour(0.0f, m_Colours.front()->getPointColour());
    m_Gradient.addColour(1.0f, m_Colours.back()->getPointColour());

    for (auto& colour : m_Colours)
    {
        m_Track.addAndMakeVisible(*colour);
        colour->addChangeListener(this);
    }
}

void meta::GradientDesigner::refreshGradient()
{
    m_Gradient.clearColours();
    std::sort(
        m_Colours.begin(),
        m_Colours.end(),
        [](auto& a, auto& b){ return a->getProportionAlongGradient() < b->getProportionAlongGradient(); }
    );

    if (m_Colours.front()->getProportionAlongGradient() != 0) { m_Gradient.addColour(0, m_Colours.front()->getPointColour()); }
    for (auto& cp : m_Colours) { m_Gradient.addColour(cp->getProportionAlongGradient(), cp->getPointColour()); }
    if (m_Colours.back()->getProportionAlongGradient() != 1) { m_Gradient.addColour(1, m_Colours.back()->getPointColour()); }

    m_Display.repaint();
    sendChangeMessage();
}

void meta::GradientDesigner::resized()
{
    auto local_bounds = getLocalBounds();
    auto display_bounds = local_bounds.removeFromTop(icon_size);
    auto track_bounds = local_bounds.removeFromTop(icon_size);

    m_Display.setBounds(display_bounds);
    m_Track.setBounds(track_bounds);

    const auto track_center_left = juce::Point<float>(m_Track.getLocalBounds().getTopLeft().x, m_Track.getLocalBounds().getCentre().y);
    const auto half_icon_size = icon_size / 2.0f;
    const auto track_width = m_Track.getWidth() - icon_size;

    for (auto& cp : m_Colours)
    {
        const auto position = cp->getProportionAlongGradient() * track_width;
        cp->setSize(icon_size, icon_size);
        cp->setCentrePosition(position + track_center_left.x + half_icon_size, track_center_left.y);
    }

    m_Gradient.point1 = m_Track.getLocalBounds().toFloat().getTopLeft();
    m_Gradient.point1 += juce::Point<float>(half_icon_size, 0);
    m_Gradient.point2 = m_Track.getLocalBounds().toFloat().getTopRight() - juce::Point<float>(half_icon_size, 0);
}

void meta::GradientDesigner::mouseDown(const juce::MouseEvent& event)
{
    if (event.eventComponent == &m_Track && event.getNumberOfClicks() > 1)
    {
        const auto x = juce::jlimit<float>(icon_size / 2.0f, m_Track.getWidth() - (icon_size / 2.0f), event.x);
        const auto proportion = x / float(m_Track.getWidth() - icon_size);
        addPoint(m_Gradient.getColourAtPosition(proportion), proportion);
        m_Colours.back()->setCentrePosition(x, icon_size / 2.0f);
    }
}

void meta::GradientDesigner::changeListenerCallback(juce::ChangeBroadcaster* source) { refreshGradient(); }

void meta::GradientDesigner::addPoint(const juce::Colour& colour, float position)
{
    jassert(position >= 0 && position <= 1);
    m_Gradient.addColour(position, colour);
    m_Colours.emplace_back(std::make_unique<ColourPoint>(colour, position));
    auto& cp = m_Colours.back();
    cp->setSize(icon_size, icon_size);
    cp->addChangeListener(this);
    m_Track.addAndMakeVisible(cp.get());
}

void meta::GradientDesigner::removePoint(meta::GradientDesigner::ColourPoint* toRemove)
{
    // We at least need a start and an end
    if (m_Colours.size() <= 2) { return; }

    for (int i = 0; i < m_Colours.size(); i++)  // noqa. This is so we can iterate and remove at the same time.
    {
        const auto point = m_Colours.at(i).get();
        if (point == toRemove)
        {
            m_Track.removeChildComponent(point);
            m_Colours.erase(m_Colours.begin() + i);
            break;
        }
    }
}

void meta::GradientDesigner::setGradient(const juce::ColourGradient& gradient)
{
    m_Colours.clear();
    m_Gradient.clearColours();

    int firstUniqueColourIndex = 0;
    int lastUniqueColourIndex = gradient.getNumColours() - 1;

    for (;
        gradient.getColour(firstUniqueColourIndex) == gradient.getColour(firstUniqueColourIndex + 1)
        && firstUniqueColourIndex < gradient.getNumColours();
        firstUniqueColourIndex++)
    {}
    m_Gradient.addColour(0, gradient.getColour(0));

    for (;
        gradient.getColour(lastUniqueColourIndex) == gradient.getColour(lastUniqueColourIndex - 1)
        && lastUniqueColourIndex > 1;
        lastUniqueColourIndex--)
    {}

    for (int i = firstUniqueColourIndex; i <= lastUniqueColourIndex; i++)
    {
        const auto c = gradient.getColour(i);
        const auto p = gradient.getColourPosition(i);

        addPoint(c, p);
    }

    resized();
}

