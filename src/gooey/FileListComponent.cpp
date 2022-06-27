/*
  ==============================================================================

   This file was originally part of the JUCE library but has been modified.
  ==============================================================================
*/
#include <meta/gooey/FileListComponent.h>

namespace juce
{
    // Forward declaration of native functions in the juce library
    juce::Image juce_createIconForFile(const juce::File& file);
}

namespace meta
{



//==============================================================================
FileListComponent::FileListComponent(juce::DirectoryContentsList& listToShow)
    : ListBox ({}, nullptr),
      DirectoryContentsDisplayComponent(listToShow),
      lastDirectory (listToShow.getDirectory())
{
    setTitle ("Files");
    setModel (this);
    directoryContentsList.addChangeListener(this);
}

FileListComponent::~FileListComponent()
{
    directoryContentsList.removeChangeListener(this);
}

int FileListComponent::getNumSelectedFiles() const
{
    return getNumSelectedRows();
}

juce::File FileListComponent::getSelectedFile(int index) const
{
    return directoryContentsList.getFile (getSelectedRow (index));
}

void FileListComponent::deselectAllFiles()
{
    deselectAllRows();
}

void FileListComponent::scrollToTop()
{
    getVerticalScrollBar().setCurrentRangeStart (0);
}

void FileListComponent::setSelectedFile(const juce::File& f)
{
    for (int i = directoryContentsList.getNumFiles(); --i >= 0;)
    {
        if (directoryContentsList.getFile (i) == f)
        {
            fileWaitingToBeSelected = juce::File();

            selectRow (i);
            return;
        }
    }

    deselectAllRows();
    fileWaitingToBeSelected = f;
}

//==============================================================================
void FileListComponent::changeListenerCallback(juce::ChangeBroadcaster*)
{
    updateContent();

    if (lastDirectory != directoryContentsList.getDirectory())
    {
        fileWaitingToBeSelected = juce::File();
        lastDirectory = directoryContentsList.getDirectory();
        deselectAllRows();
    }

    if (fileWaitingToBeSelected != juce::File())
        setSelectedFile (fileWaitingToBeSelected);
}

//==============================================================================
class FileListComponent::ItemComponent
    : public juce::Component
    , private juce::TimeSliceClient
    , private juce::AsyncUpdater
{
public:
    ItemComponent (FileListComponent& fc, juce::TimeSliceThread& t)
        : owner (fc)
        , thread (t)
    {}

    ~ItemComponent() override
        { thread.removeTimeSliceClient (this); }

    //==============================================================================
    void paint(juce::Graphics& g) override
    {
        getLookAndFeel().drawFileBrowserRow(
            g, getWidth(), getHeight(),
            file, file.getFileName(),
            &icon, fileSize, modTime,
            isDirectory, highlighted,
            index, owner
        );
    }

    void mouseDown(const juce::MouseEvent& e) override
    {
        owner.selectRowsBasedOnModifierKeys (index, e.mods, true);
        owner.sendMouseClickMessage (file, e);
    }

    void mouseDoubleClick (const juce::MouseEvent&) override
    {
        owner.sendDoubleClickMessage (file);
    }

    void update(const juce::File& root, const juce::DirectoryContentsList::FileInfo* fileInfo,
                 int newIndex, bool nowHighlighted)
    {
        thread.removeTimeSliceClient (this);

        if (nowHighlighted != highlighted || newIndex != index)
        {
            index = newIndex;
            highlighted = nowHighlighted;
            repaint();
        }

        juce::File newFile;
        juce::String newFileSize, newModTime;

        if (fileInfo != nullptr)
        {
            newFile = root.getChildFile (fileInfo->filename);
            newFileSize = juce::File::descriptionOfSizeInBytes (fileInfo->fileSize);
            newModTime = fileInfo->modificationTime.formatted ("%d %b '%y %H:%M");
        }

        if (newFile != file
             || fileSize != newFileSize
             || modTime != newModTime)
        {
            file = newFile;
            fileSize = newFileSize;
            modTime = newModTime;
            icon = juce::Image();
            isDirectory = fileInfo != nullptr && fileInfo->isDirectory;

            repaint();
        }

        if (file != juce::File() && icon.isNull() && ! isDirectory)
        {
            updateIcon (true);

            if (! icon.isValid())
                thread.addTimeSliceClient (this);
        }
    }

    int useTimeSlice() override
    {
        updateIcon (false);
        return -1;
    }

    void handleAsyncUpdate() override
    {
        repaint();
    }

private:
    //==============================================================================
    FileListComponent& owner;
    juce::TimeSliceThread& thread;
    juce::File file;
    juce::String fileSize, modTime;
    juce::Image icon;
    int index = 0;
    bool highlighted = false, isDirectory = false;

    std::unique_ptr<juce::AccessibilityHandler> createAccessibilityHandler() override
    {
        return createIgnoredAccessibilityHandler (*this);
    }

    void updateIcon (const bool onlyUpdateIfCached)
    {
        if (icon.isNull())
        {
            auto hashCode = (file.getFullPathName() + "_iconCacheSalt").hashCode();
            auto im = juce::ImageCache::getFromHashCode (hashCode);

            if (im.isNull() && ! onlyUpdateIfCached)
            {
                im = juce::juce_createIconForFile (file);

                if (im.isValid())
                    juce::ImageCache::addImageToCache (im, hashCode);
            }

            if (im.isValid())
            {
                icon = im;
                triggerAsyncUpdate();
            }
        }
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ItemComponent)
};

//==============================================================================
int FileListComponent::getNumRows()
{
    return directoryContentsList.getNumFiles();
}

juce::String FileListComponent::getNameForRow (int rowNumber)
{
    return directoryContentsList.getFile (rowNumber).getFileName();
}

void FileListComponent::paintListBoxItem(int, juce::Graphics&, int, int, bool){}

juce::Component* FileListComponent::refreshComponentForRow (int row, bool isSelected, Component* existingComponentToUpdate)
{
    jassert (existingComponentToUpdate == nullptr || dynamic_cast<ItemComponent*> (existingComponentToUpdate) != nullptr);

    auto comp = static_cast<ItemComponent*> (existingComponentToUpdate);

    if (comp == nullptr)
        comp = new ItemComponent (*this, directoryContentsList.getTimeSliceThread());

    juce::DirectoryContentsList::FileInfo fileInfo;
    comp->update (directoryContentsList.getDirectory(),
                  directoryContentsList.getFileInfo (row, fileInfo) ? &fileInfo : nullptr,
                  row, isSelected);

    return comp;
}

void FileListComponent::selectedRowsChanged (int /*lastRowSelected*/)
{
    sendSelectionChangeMessage();
}

void FileListComponent::deleteKeyPressed (int /*currentSelectedRow*/)
{
}

void FileListComponent::returnKeyPressed (int currentSelectedRow)
{
    sendDoubleClickMessage (directoryContentsList.getFile (currentSelectedRow));
}

} // namespace meta
