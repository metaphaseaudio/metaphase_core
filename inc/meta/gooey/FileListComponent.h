/*
  ==============================================================================

   This file was originally part of the JUCE library, but has been modified.
  ==============================================================================
*/

#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

namespace meta
{

//==============================================================================
/**
    A component that displays the files in a directory as a listbox.

    This implements the DirectoryContentsDisplayComponent base class so that
    it can be used in a FileBrowserComponent.

    To attach a listener to it, use its DirectoryContentsDisplayComponent base
    class and the FileBrowserListener class.

    @see DirectoryContentsList, FileTreeComponent

    @tags{GUI}
*/
class FileListComponent
    : public juce::ListBox
    , public juce::DirectoryContentsDisplayComponent
    , private juce::ListBoxModel
    , private juce::ChangeListener
{
public:
    //==============================================================================
    /** Creates a listbox to show the contents of a specified directory. */
    explicit FileListComponent(juce::DirectoryContentsList& listToShow);

    /** Destructor. */
    ~FileListComponent() override;

    //==============================================================================
    /** Returns the number of files the user has got selected.
        @see getSelectedFile
    */
    [[nodiscard]] int getNumSelectedFiles() const override;

    /** Returns one of the files that the user has currently selected.
        The index should be in the range 0 to (getNumSelectedFiles() - 1).
        @see getNumSelectedFiles
    */
    [[nodiscard]] juce::File getSelectedFile(int index) const override;

    /** Deselects any files that are currently selected. */
    void deselectAllFiles() override;

    /** Scrolls to the top of the list. */
    void scrollToTop() override;

    /** If the specified file is in the list, it will become the only selected item
        (and if the file isn't in the list, all other items will be deselected). */
    void setSelectedFile (const juce::File&) override;

private:
    //==============================================================================
    juce::File lastDirectory, fileWaitingToBeSelected;
    class ItemComponent;

    void changeListenerCallback(juce::ChangeBroadcaster*) override;
    int getNumRows() override;
    juce::String getNameForRow(int rowNumber) override;
    void paintListBoxItem(int, juce::Graphics&, int, int, bool) override;
    Component* refreshComponentForRow(int rowNumber, bool isRowSelected, Component*) override;
    void selectedRowsChanged(int row) override;
    void deleteKeyPressed(int currentSelectedRow) override;
    void returnKeyPressed(int currentSelectedRow) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FileListComponent)
};

} // namespace meta
