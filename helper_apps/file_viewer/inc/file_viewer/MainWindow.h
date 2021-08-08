//
// Created by Matt on 8/8/2021.
//
#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include <file_viewer/MainComponent.h>


class MainWindow
    : public juce::DocumentWindow
    , public juce::MenuBarModel
    , public juce::ApplicationCommandTarget
    , public juce::FileDragAndDropTarget
{
public:
    explicit MainWindow (juce::String name);
    ~MainWindow();
    void closeButtonPressed() override;

    // Dragon Drop implementation
    bool isInterestedInFileDrag(const juce::StringArray& files) override { return true; };
    void fileDragEnter(const juce::StringArray& files, int, int) override {};
    void fileDragMove(const juce::StringArray& files, int, int) override {};
    void fileDragExit(const juce::StringArray& files) override {};
    void filesDropped(const juce::StringArray& files, int, int) override;

    // Menu bar implementation
    void menuBarActivated(bool isActive) override;
    juce::StringArray getMenuBarNames() override;
    juce::PopupMenu getMenuForIndex(int topLevelMenuIndex, const juce::String& menuName) override;
    void menuItemSelected(int menuItemID, int topLevelMenuIndex) override;

    // Application command implementation
    ApplicationCommandTarget* getNextCommandTarget() override;
    void getAllCommands(juce::Array<int>&) override;
    void getCommandInfo(int, juce::ApplicationCommandInfo&) override;
    bool perform(const InvocationInfo&) override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
};
