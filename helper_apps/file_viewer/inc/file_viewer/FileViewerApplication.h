//
// Created by Matt on 8/8/2021.
//

#pragma once

#include <file_viewer/MainComponent.h>
#include <file_viewer/MainWindow.h>

//==============================================================================
class FileViewerApplication  : public juce::JUCEApplication
{
public:
    //==============================================================================
    FileViewerApplication() {}
    const juce::String getApplicationName() override       { return JUCE_APPLICATION_NAME_STRING; }
    const juce::String getApplicationVersion() override    { return JUCE_APPLICATION_VERSION_STRING; }
    bool moreThanOneInstanceAllowed() override             { return true; }

    //==============================================================================
    void initialise (const juce::String& commandLine) override;

    void shutdown() override;

    //==============================================================================
    void systemRequestedQuit() override;

    void anotherInstanceStarted(const juce::String& commandLine) override;

    juce::ApplicationCommandManager commandManager;
    std::unique_ptr<juce::ApplicationProperties> appProperties;
    std::unique_ptr<MainWindow> mainWindow;
};

static FileViewerApplication& getApp()               { return *dynamic_cast<FileViewerApplication*>(juce::JUCEApplication::getInstance()); }
juce::ApplicationProperties& getAppProperties()      { return *getApp().appProperties; }
juce::ApplicationCommandManager& getCommandManager() { return getApp().commandManager; }
//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (FileViewerApplication)
