//
// Created by Matt on 9/9/2021.
//

#include <meta/gooey/GradientDesigner.h>
#include <meta/gooey/MetaLookAndFeel.h>
#include <juce_gui_extra/juce_gui_extra.h>
#include <inc/meta/util/container_helpers/array.h>

meta::GradientDesigner::Display::Display(const juce::ColourGradient& gradient)
    : r_Gradient(gradient)
{}

void meta::GradientDesigner::Display::paint(juce::Graphics& g)
{
    g.setGradientFill(r_Gradient);
    g.fillRoundedRectangle(getLocalBounds().reduced(3).toFloat(), getHeight() / 2.0f);
    g.setColour(juce::Colours::darkgrey.darker());
    g.drawRoundedRectangle(getLocalBounds().reduced(3).toFloat(), getHeight() / 2.0f, 3.0f);
}

///////////////////////////////////////////////////////////////////////////////
meta::GradientDesigner::ColourPoint::ColourPoint(juce::Colour colour)
    : m_Colour(colour)
{}

void meta::GradientDesigner::ColourPoint::paint(juce::Graphics& g)
    { dynamic_cast<meta::MetaLookAndFeel*>(&getLookAndFeel())->drawGradientColourPoint(g, *this); }

void meta::GradientDesigner::ColourPoint::resized()
    { m_Constraints.setMinimumOnscreenAmounts(getHeight(), getWidth(), getHeight(), getWidth()); }

void meta::GradientDesigner::ColourPoint::mouseDown(const juce::MouseEvent& e)
{
    auto clickCount = e.getNumberOfClicks();
    if (clickCount > 1)
    {
        auto colourSelector = std::make_unique<juce::ColourSelector>
                (juce::ColourSelector::showColourAtTop | juce::ColourSelector::showSliders | juce::ColourSelector::showColourspace);
        colourSelector->setSize(300, 300);
        colourSelector->setCurrentColour(m_Colour, juce::dontSendNotification);
        colourSelector->addChangeListener(this);

        auto& box = juce::CallOutBox::launchAsynchronously(std::move(colourSelector), getScreenBounds(), nullptr);
    }
    else { drag_context.startDraggingComponent (this, e); }
}

void meta::GradientDesigner::ColourPoint::mouseDrag(const juce::MouseEvent& e) { drag_context.dragComponent (this, e, &m_Constraints); }

void meta::GradientDesigner::ColourPoint::mouseUp(const juce::MouseEvent& event) { sendChangeMessage(); }

void meta::GradientDesigner::ColourPoint::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    auto callout = dynamic_cast<juce::ColourSelector*>(source);
    m_Colour = callout->getCurrentColour();
    sendChangeMessage();
    repaint();
}

float meta::GradientDesigner::ColourPoint::getColourPosition() const
    { return getLocalBounds().toFloat().getCentre().x / getParentComponent()->getLocalBounds().toFloat().getWidth(); }

///////////////////////////////////////////////////////////////////////////////
meta::GradientDesigner::GradientDesigner()
    : m_Display(m_Gradient)
{
    addAndMakeVisible(m_Display);
    addAndMakeVisible(m_Track);

    // Setup default gradient
    m_Gradient.isRadial = false;
    m_Colours.emplace_back(std::make_unique<ColourPoint>(juce::Colours::red));
    m_Colours.emplace_back(std::make_unique<ColourPoint>(juce::Colours::green));
    m_Gradient.addColour(0.0f, m_Colours.back()->getPointColour());
    m_Gradient.addColour(1.0f, m_Colours.begin()->get()->getPointColour());

    for (auto& colour : m_Colours)
    {
        m_Track.addAndMakeVisible(*colour);
        colour->addChangeListener(this);
    }
}

void meta::GradientDesigner::resized()
{
    const auto point_size = 22;
    auto local_bounds = getLocalBounds();
    auto display_bounds = local_bounds.removeFromTop(point_size + 6);  // TODO: get rid of the +6 and get the border from the display
    auto track_bounds = local_bounds.removeFromTop(point_size);

    m_Display.setBounds(display_bounds);
    m_Track.setBounds(track_bounds);

    const auto track_center_left = juce::Point<float>(m_Track.getLocalBounds().getTopLeft().x, m_Track.getLocalBounds().getCentre().y);
    const auto half_point_size = point_size / 2.0f;
    const auto track_width = m_Track.getWidth() - point_size;

    for (auto colour_int : meta::enumerate(m_Colours))
    {
        // Gradients require something at 0 and 1 mechanically, but visually
        // we don't, so now we need to reconcile that OB1.
        const auto index = std::get<0>(colour_int) + 1;
        const auto position = m_Gradient.getColourPosition(index);
        std::get<1>(colour_int)->setSize(point_size, point_size);

        // The 'end' of the track is width - the token size, everything else we can be judged on the 'position' of the pointer (which is
        std::get<1>(colour_int)->setCentrePosition(float(position * track_width) + track_center_left.x + half_point_size, track_center_left.y);
    }

    m_Gradient.point1 = m_Track.getLocalBounds().toFloat().getTopLeft();
    m_Gradient.point1 += juce::Point<float>(half_point_size, 0);
    m_Gradient.point2 = m_Track.getLocalBounds().toFloat().getTopRight() - juce::Point<float>(half_point_size, 0);
}

void meta::GradientDesigner::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    std::sort(m_Colours.begin(), m_Colours.end(), [](auto& a, auto& b){ return a->getPosition().getX() < b->getPosition().getX(); });
    m_Gradient.clearColours();
    m_Gradient.addColour(0, m_Colours.front()->getPointColour());
    m_Gradient.addColour(1, m_Colours.back()->getPointColour());

    for (auto& colour_pointer : m_Colours)
    {
        float proportion = colour_pointer->getPosition().toFloat().getX() / float(m_Track.getWidth() - colour_pointer->getWidth());
        if (proportion == 0) { proportion += std::numeric_limits<float>::epsilon(); }
        if (proportion == 1) { proportion -= std::numeric_limits<float>::epsilon(); }

        m_Gradient.addColour(proportion, colour_pointer->getPointColour());
    }

    m_Display.repaint();
}
