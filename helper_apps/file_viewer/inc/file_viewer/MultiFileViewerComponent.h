//
// Created by Matt on 8/9/2021.
//

#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_basics/juce_audio_basics.h>
#include "FileViewerComponent.h"


class MultiFileViewerComponent
    : public juce::Component
{
public:
    MultiFileViewerComponent();
    void paint (juce::Graphics&) override;
    void resized() override;

    void addFile(const juce::File& filepath, juce::AudioBuffer<float>& data, double sample_rate);
    void removeFile(const juce::File& filepath);
    juce::RecentlyOpenedFilesList listFiles() const;
    bool fileIsOpen(const juce::File& filepath) const;

private:
    FileViewerComponent* const* findViewer(const juce::File& filepath) const;
    juce::OwnedArray<FileViewerComponent> m_Views;
    juce::TabbedComponent m_TabHandler;
};


