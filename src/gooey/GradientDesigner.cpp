//
// Created by Matt on 9/9/2021.
//

#include <meta/gooey/GradientDesigner.h>
#include <meta/gooey/MetaLookAndFeel.h>
#include <juce_gui_extra/juce_gui_extra.h>

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


meta::GradientDesigner::ColourPoint::ColourPoint(juce::Colour colour)
    : m_Colour(colour)
{}


void meta::GradientDesigner::ColourPoint::paint(juce::Graphics& g)
{
    dynamic_cast<meta::MetaLookAndFeel*>(&getLookAndFeel())->drawGradientColourPoint(g, *this);
}

void meta::GradientDesigner::ColourPoint::resized()
{
    m_Constraints.setMinimumOnscreenAmounts(getHeight(), getWidth(), getHeight(), getWidth());
}

void meta::GradientDesigner::ColourPoint::mouseDown(const juce::MouseEvent& e)
{
    auto clickCount = e.getNumberOfClicks();
    if (clickCount > 1)
    {
        auto colourSelector = std::make_unique<juce::ColourSelector>();
        colourSelector->setSize(300, 300);

        colourSelector->setCurrentColour(m_Colour, juce::dontSendNotification);
        colourSelector->addChangeListener(this);

        auto& box = juce::CallOutBox::launchAsynchronously(std::move(colourSelector), getScreenBounds(), nullptr);
    }
    else
    {
        drag_context.startDraggingComponent (this, e);
    }
}

void meta::GradientDesigner::ColourPoint::mouseDrag(const juce::MouseEvent& e)
{
    drag_context.dragComponent (this, e, &m_Constraints);
}

void meta::GradientDesigner::ColourPoint::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    auto callout = dynamic_cast<juce::ColourSelector*>(source);
    m_Colour = callout->getCurrentColour();
    repaint();
}

void meta::GradientDesigner::ColourPoint::mouseUp(const juce::MouseEvent& event)
{
    sendChangeMessage();
}


meta::GradientDesigner::GradientDesigner()
    : m_Display(m_Gradient)
    , m_Point(juce::Colours::red)
    , m_Point_2(juce::Colours::green)
{
    m_Gradient.isRadial = false;
    m_Gradient.addColour(0.0f, juce::Colours::red);
    m_Gradient.addColour(1.0f, juce::Colours::blue);
    addAndMakeVisible(m_Display);
    addAndMakeVisible(m_Track);
    m_Track.addAndMakeVisible(m_Point);
    m_Track.addAndMakeVisible(m_Point_2);
}

void meta::GradientDesigner::resized()
{
    const auto track_size = 22;
    auto local_bounds = getLocalBounds();
    auto track_bounds = local_bounds.removeFromBottom(track_size);
    track_bounds.removeFromLeft((local_bounds.getHeight() / 2.0f) - (m_Point.getWidth() / 2.0f));
    track_bounds.removeFromRight((local_bounds.getHeight() / 2.0f) - (m_Point.getWidth() / 2.0f));

    m_Display.setBounds(local_bounds);
    m_Gradient.point1 = m_Track.getBounds().toFloat().getTopLeft();
    m_Gradient.point2 = m_Track.getBounds().toFloat().getTopRight();
    m_Track.setBounds(track_bounds);
    m_Point.setSize(track_size, track_size);
    m_Point_2.setSize(track_size, track_size);
    m_Point.setCentrePosition(m_Track.getLocalBounds().getCentre());
    m_Point_2.setCentrePosition(m_Track.getLocalBounds().getCentre());
}
