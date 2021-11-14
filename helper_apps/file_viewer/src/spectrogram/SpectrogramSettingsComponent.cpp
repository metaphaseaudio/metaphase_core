//
// Created by Matt on 9/6/2021.
//

#include <file_viewer/spectrogram/SpectrogramSettingsComponent.h>


SpectrogramSettingsComponent::SpectrogramSettingsComponent(SpectrogramSettings& settings)
    : r_Settings(settings)
    , m_SelectScaleLabel("Freq Scale", "Frequency Scale:")
{
    addAndMakeVisible(m_GradientDesigner);
    addAndMakeVisible(m_SelectScaleLabel); addAndMakeVisible(m_SelectScale);
    m_GradientDesigner.setGradient(r_Settings.getGradient());
    m_GradientDesigner.addChangeListener(this);

    m_SelectScale.addItemList({"Linear", "Log", "Mel"}, 1);
    m_SelectScale.setSelectedId(r_Settings.getScale() + 1, juce::dontSendNotification);
    m_SelectScale.addListener(this);
    setSize(700, 100);
}

void SpectrogramSettingsComponent::resized()
{
    auto localBounds = getLocalBounds();
    m_GradientDesigner.setBounds(localBounds.removeFromTop(44));
    localBounds.removeFromTop(2);
    auto rowBounds = localBounds.removeFromTop(22);
    m_SelectScaleLabel.setBounds(rowBounds.removeFromLeft(localBounds.getWidth() / 2));
    m_SelectScale.setBounds(rowBounds);
    localBounds.removeFromTop(2);
}

void SpectrogramSettingsComponent::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    if (source == &m_GradientDesigner) { r_Settings.setGradient(m_GradientDesigner.getGradient()); }
}

void SpectrogramSettingsComponent::comboBoxChanged(juce::ComboBox* comboBoxThatHasChanged)
{
    if (comboBoxThatHasChanged == &m_SelectScale)
        { r_Settings.setScale(SpectrogramSettings::Scale(m_SelectScale.getSelectedId() - 1)); }
}
