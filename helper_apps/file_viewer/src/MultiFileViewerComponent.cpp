//
// Created by Matt on 8/9/2021.
//

#include <file_viewer/MultiFileViewerComponent.h>
#include <file_viewer/TabCloseButton.h>


MultiFileViewerComponent::MultiFileViewerComponent()
    : m_TabHandler(juce::TabbedButtonBar::TabsAtTop)
{
    setSize (1000, 800);
    addAndMakeVisible(m_TabHandler);
}

//==============================================================================
void MultiFileViewerComponent::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
    g.setFont (juce::Font (16.0f));
    g.setColour (juce::Colours::grey);
    g.drawText ("Drag and drop files here to view them", getLocalBounds(), juce::Justification::centred, true);
}

void MultiFileViewerComponent::resized()
{
    m_TabHandler.setBounds(getLocalBounds());
}

void MultiFileViewerComponent::addFile(const juce::File& filepath, juce::AudioBuffer<float>& data, double sample_rate)
{
    auto to_show = m_Views.add(new FileViewerComponent(filepath, data, sample_rate, spectrogramSettings));
    m_TabHandler.addTab(filepath.getFileName(), juce::Colours::darkgrey, to_show, false);
    const juce::String tab_name = filepath.getFileName();
    auto btn = m_TabHandler.getTabbedButtonBar().getTabButton(m_TabHandler.getNumTabs() - 1);
    auto* close_btn = new TabCloseButton(tab_name);
    close_btn->onClick = [this, filepath](){ removeFile(filepath); };
    btn->setExtraComponent(close_btn, juce::TabBarButton::ExtraComponentPlacement::afterText);
}

FileViewerComponent* const* MultiFileViewerComponent::findViewer(const juce::File& filepath) const
{
    return std::find_if(m_Views.begin(), m_Views.end(), [&](const FileViewerComponent* x){ return x->getFilepath() == filepath; });
}

bool MultiFileViewerComponent::fileIsOpen(const juce::File& filepath) const
{
    const auto found = findViewer(filepath);
    return found != nullptr && found != m_Views.end();
}

void MultiFileViewerComponent::viewFile(const juce::File& filepath)
{
    auto found = findViewer(filepath);
    const auto index = found - m_Views.begin();
    m_TabHandler.setCurrentTabIndex(index);
}

void MultiFileViewerComponent::removeFile(const juce::File& filepath)
{
    const auto found = findViewer(filepath);
    jassert(found != nullptr);

    const auto i = m_Views.indexOf(*found);
    m_TabHandler.removeTab(i);
    m_Views.remove(i);
}

juce::RecentlyOpenedFilesList MultiFileViewerComponent::listFiles() const
{
    juce::RecentlyOpenedFilesList files;
    for (auto view : m_Views) { files.addFile(view->getFilepath()); }
    return files;
}

