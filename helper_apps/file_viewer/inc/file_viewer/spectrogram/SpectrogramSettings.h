//
// Created by Matt on 9/19/2021.
//

#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

class SpectrogramSettings
{
public:
    class Listener
    {
    public:
        virtual void gradientChanged(const SpectrogramSettings* settings) = 0;
        virtual void fftChanged(const SpectrogramSettings* settings) = 0;
    };

    SpectrogramSettings();

    void addListener(Listener* listener) { m_Listeners.add(listener); };
    void removeListener(Listener* listener) { m_Listeners.remove(listener); };

    void setGradient(const juce::ColourGradient& gradient);
    juce::ColourGradient getGradient() const { return m_Gradient; };

private:
    juce::ListenerList<Listener> m_Listeners;
    juce::ColourGradient m_Gradient;
};


