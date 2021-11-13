//
// Created by Matt on 9/6/2021.
//

#include <file_viewer/spectrogram/SpectrogramSettingsComponent.h>


SpectrogramSettingsComponent::SpectrogramSettingsComponent(SpectrogramSettings& settings)
    : r_Settings(settings)
{
    addAndMakeVisible(m_GradientDesigner);
    m_GradientDesigner.setGradient(r_Settings.getGradient());
    m_GradientDesigner.addChangeListener(this);
    setSize(700, 100);
}

void SpectrogramSettingsComponent::resized() { m_GradientDesigner.setBounds(getLocalBounds().reduced(5)); }

void SpectrogramSettingsComponent::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    if (source == &m_GradientDesigner) { r_Settings.setGradient(m_GradientDesigner.getGradient()); }
}
