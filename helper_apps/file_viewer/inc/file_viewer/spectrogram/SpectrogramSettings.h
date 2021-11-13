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

    enum Scale
    {
        LIN,
        LOG,
        MEL
    };

    SpectrogramSettings();

    void addListener(Listener* listener) { m_Listeners.add(listener); };
    void removeListener(Listener* listener) { m_Listeners.remove(listener); };

    void setGradient(const juce::ColourGradient& gradient);
    const juce::ColourGradient& getGradient() const { return m_Gradient; };

    void setScale(const Scale& scale);
    Scale getScale() const { return m_Scale; }

    void setFFTOrder(int fftOrder);
    int getFFTOrder() const { return m_FFTOrder; }
    int getFFTSize() const { return std::pow(2, m_FFTOrder); }

    void setXOverlap(int overlap);
    int getXOverlap() const { return m_XOverlap; }

private:

    juce::ListenerList<Listener> m_Listeners;
    int m_FFTOrder, m_XOverlap;
    juce::ColourGradient m_Gradient;
    Scale m_Scale;
};


