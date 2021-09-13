//
// Created by Matt on 9/6/2021.
//

#include <file_viewer/spectrogram/SpectrogramSettingsWindow.h>
#include <file_viewer/spectrogram/SpectrogramSettingsComponent.h>


SpectrogramSettingsWindow::SpectrogramSettingsWindow()
    : juce::DocumentWindow("Spectrogram Settings", juce::Colours::grey, juce::DocumentWindow::TitleBarButtons::closeButton)
{
    setContentOwned(new SpectrogramSettingsComponent(), true);
    centreWithSize(getWidth(), getHeight());
}


void SpectrogramSettingsWindow::closeButtonPressed()
{
    clearContentComponent();
    setVisible(false);
}

