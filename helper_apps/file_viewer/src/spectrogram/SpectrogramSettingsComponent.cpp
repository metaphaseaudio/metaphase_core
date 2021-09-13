//
// Created by Matt on 9/6/2021.
//

#include <file_viewer/spectrogram/SpectrogramSettingsComponent.h>


SpectrogramSettingsComponent::SpectrogramSettingsComponent()
{
    addAndMakeVisible(m_Gradient);
    setSize(100, 100);
}

void SpectrogramSettingsComponent::resized()
{
    m_Gradient.setBounds(getLocalBounds().reduced(10));
}
