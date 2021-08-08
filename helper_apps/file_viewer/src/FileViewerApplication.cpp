//
// Created by Matt on 8/8/2021.
//
#include <file_viewer/FileViewerApplication.h>

#include <memory>

void FileViewerApplication::initialise(const juce::String& commandLine)
{
    // This method is where you should put your application's initialisation code..
    juce::ignoreUnused (commandLine);
    juce::PropertiesFile::Options options;
    options.applicationName     = "Metaphase Audio File Viewer";
    options.filenameSuffix      = "settings";
    options.osxLibrarySubFolder = "Preferences";

    appProperties = std::make_unique<juce::ApplicationProperties>();
    appProperties->setStorageParameters (options);

    mainWindow = std::make_unique<MainWindow>(getApplicationName());

    commandManager.registerAllCommandsForTarget(this);
    commandManager.registerAllCommandsForTarget(mainWindow.get());
}


void FileViewerApplication::shutdown()
{
    mainWindow = nullptr;
}


void FileViewerApplication::systemRequestedQuit()
{
    // This is called when the app is being asked to quit: you can ignore this
    // request and let the app carry on running, or call quit() to allow the app to close.
    quit();
}


void FileViewerApplication::anotherInstanceStarted(const juce::String& commandLine)
{
    // When another instance of the app is launched while this one is running,
    // this method is invoked, and the commandLine parameter tells you what
    // the other instance's command-line arguments were.
    juce::ignoreUnused (commandLine);
}
