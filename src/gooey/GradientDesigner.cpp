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

///////////////////////////////////////////////////////////////////////////////


meta::GradientDesigner::GradientDesigner()
    : m_Display(m_Gradient)
{
    m_Colours.emplace_back(std::make_unique<ColourPoint>(juce::Colours::red));
    m_Colours.emplace_back(std::make_unique<ColourPoint>(juce::Colours::green));
    m_Gradient.isRadial = false;
    m_Gradient.addColour(0.0f, m_Colours.begin()->get()->getPointColour());
    m_Gradient.addColour(1.0f, m_Colours.back()->getPointColour());
    addAndMakeVisible(m_Display);
    addAndMakeVisible(m_Track);

    for (auto& colour : m_Colours)
    {
        m_Track.addAndMakeVisible(*colour.get());
        colour->addChangeListener(this);
    }
}

void meta::GradientDesigner::resized()
{
    const auto track_size = 22;
    auto local_bounds = getLocalBounds();
    auto display_bounds = local_bounds.removeFromTop(track_size + 6);
    auto track_bounds = local_bounds.removeFromTop(track_size);
    track_bounds.removeFromLeft((local_bounds.getHeight() / 2.0f) - (track_size / 2.0f));
    track_bounds.removeFromRight((local_bounds.getHeight() / 2.0f) - (track_size / 2.0f));

    m_Display.setBounds(display_bounds);
    m_Track.setBounds(track_bounds);

    for (auto& colour : m_Colours)
    {
        colour->setSize(track_size, track_size);
        colour->setCentrePosition(m_Track.getLocalBounds().getCentre());
    }

    m_Gradient.point1 = m_Track.getBounds().toFloat().getTopLeft();
    m_Gradient.point2 = m_Track.getBounds().toFloat().getTopRight();
}

void meta::GradientDesigner::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    std::vector<std::tuple<juce::Colour, float>> colour_stops;

    for (auto& colour_pointer : m_Colours)
    {
        const float proportion = m_Track.getPosition().toFloat().getX() / colour_pointer->getPosition().toFloat().getX();
        colour_stops.emplace_back(std::make_tuple(colour_pointer->getPointColour(), proportion));
    }

    std::sort(colour_stops.begin(), colour_stops.end(), [](auto& a, auto& b){ return std::get<1>(a) < std::get<1>(b); });

    m_Gradient.clearColours();
    m_Gradient.addColour(0, std::get<0>(colour_stops.front()));
    m_Gradient.addColour(1, std::get<0>(colour_stops.back()));
    for (auto& stop : colour_stops) { m_Gradient.addColour(std::get<1>(stop), std::get<0>(stop)); }
    m_Display.repaint();
}
