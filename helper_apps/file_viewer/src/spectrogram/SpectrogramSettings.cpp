//
// Created by Matt on 9/19/2021.
//

#include <file_viewer/spectrogram/SpectrogramSettings.h>


SpectrogramSettings::SpectrogramSettings()
{

}

void SpectrogramSettings::setGradient(const juce::ColourGradient& gradient)
{
    m_Gradient = gradient;
    m_Listeners.call([this] (Listener& l) { l.gradientChanged(this); });
}
