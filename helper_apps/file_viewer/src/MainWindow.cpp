//
// Created by Matt on 8/8/2021.
//

#include <file_viewer/MainWindow.h>
#include <file_viewer/constants.h>
#include <file_viewer/forward_declarations.h>


MainWindow::MainWindow(juce::String name)
    : DocumentWindow
        (name, juce::Desktop::getInstance().getDefaultLookAndFeel().findColour(ResizableWindow::backgroundColourId), DocumentWindow::allButtons)
{
    setUsingNativeTitleBar(true);
    m_ViewHandler.setLookAndFeel(&m_LAF);
    setContentNonOwned(&m_ViewHandler, true);

    #if JUCE_IOS || JUCE_ANDROID
        setFullScreen (true);
    #else
        setResizable(true, true);
        centreWithSize(getWidth(), getHeight());
    #endif

    restoreWindowStateFromString(getAppProperties().getUserSettings()->getValue(AppState::MAIN_WINDOW_POSITION));
    setVisible(true);

    addKeyListener (getCommandManager().getKeyMappings());

    #if JUCE_MAC
        setMacMainMenu (this);
    #else
        setMenuBar (this);
    #endif

    getCommandManager().setFirstCommandTarget (this);

    juce::RecentlyOpenedFilesList openedFilesList;
    openedFilesList.restoreFromString(getAppProperties().getUserSettings()->getValue(AppState::OPEN_FILES));
    for (auto filepath : openedFilesList.getAllFilenames()) { load(filepath); }
}


MainWindow::~MainWindow() noexcept
{
    m_ViewHandler.setLookAndFeel(nullptr);
    #if ! (JUCE_ANDROID || JUCE_IOS)
        #if JUCE_MAC
            setMacMainMenu (nullptr);
        #else
            setMenuBar (nullptr);
        #endif
    #endif
}
void MainWindow::closeButtonPressed()
{
    const auto file_list = m_ViewHandler.listFiles();
    getAppProperties().getUserSettings()->setValue(AppState::OPEN_FILES, file_list.toString());
    juce::JUCEApplication::getInstance()->systemRequestedQuit();
}

void MainWindow::filesDropped(const juce::StringArray& files, int, int)
{

};

// Menu bar implementation
void MainWindow::menuBarActivated (bool isActive) {};


juce::StringArray MainWindow::getMenuBarNames() { return {"File", "Options"}; };


juce::PopupMenu MainWindow::getMenuForIndex(int topLevelMenuIndex, const juce::String& menuName)
{
    juce::PopupMenu menu;
    auto& cmd_mgr = getCommandManager();
    if (topLevelMenuIndex == 0) // File
    {
        menu.addCommandItem(&cmd_mgr, CommandIDs::open_file);
        menu.addCommandItem(&cmd_mgr, CommandIDs::close_file);

        juce::RecentlyOpenedFilesList recentFiles;
        recentFiles.restoreFromString(getAppProperties().getUserSettings()->getValue (AppState::RECENT_FILES));

        juce::PopupMenu recentFilesMenu;
        recentFiles.createPopupMenuItems (recentFilesMenu, 100, true, true);
        menu.addSubMenu ("Recent files", recentFilesMenu);
    }

    if (topLevelMenuIndex == 1) // Window
    {
        menu.addCommandItem(&cmd_mgr, CommandIDs::audio_settings);
        menu.addCommandItem(&cmd_mgr, CommandIDs::spectrogram_settings);
        menu.addCommandItem(&cmd_mgr, CommandIDs::about);
    }

    return menu;
};

void MainWindow::menuItemSelected(int menuItemID, int topLevelMenuIndex)
{
    if (topLevelMenuIndex == 0) // File
    {
        if (menuItemID >= 100 && menuItemID < 200)
        {
            juce::RecentlyOpenedFilesList recentFiles;
            recentFiles.restoreFromString(getAppProperties().getUserSettings()->getValue(AppState::RECENT_FILES));
            load(recentFiles.getFile(menuItemID - 100));
        }
    }

    else if (topLevelMenuIndex == 1)  // Window
    {

    }
};

// Application command implementation
juce::ApplicationCommandTarget* MainWindow::getNextCommandTarget() { return findFirstTargetParentComponent(); };


void MainWindow::getAllCommands (juce::Array<int>& cmds)
{
    cmds.addArray({
        CommandIDs::open_file,
        CommandIDs::close_file,
        CommandIDs::audio_settings,
        CommandIDs::spectrogram_settings,
        CommandIDs::about
    });
};


void MainWindow::getCommandInfo (int cmd_id, juce::ApplicationCommandInfo& result)
{
    std::string category = "General";

    switch (cmd_id)
    {
        case CommandIDs::open_file:
            result.setInfo("Open", "Opens a file", category, 0);
            result.defaultKeypresses.add(juce::KeyPress('o', juce::ModifierKeys::commandModifier, 0));
            break;

        case CommandIDs::close_file:
            result.setInfo("Close", "Closes the currently visible file", category, 0);
            result.defaultKeypresses.add(juce::KeyPress('q', juce::ModifierKeys::commandModifier, 0));
            break;

        case CommandIDs::audio_settings:
            result.setInfo("Audio Settings", "Opens the Audio Settings Panel", category, 0);
            result.defaultKeypresses.add(juce::KeyPress(',', juce::ModifierKeys::commandModifier, 0));
            break;

        case CommandIDs::spectrogram_settings:
            result.setInfo("Spectrogram Settings", "Opens the Spectrogram Settings Panel", category, 0);
            result.defaultKeypresses.add(juce::KeyPress('s'));
            break;

        case CommandIDs::about:
            result.setInfo("About", "Opens the About Panel", category, 0);
            result.defaultKeypresses.add(juce::KeyPress(juce::KeyPress::F1Key));
            break;

        default:
            break;
    }
};

bool MainWindow::perform (const InvocationInfo& info)
{
    switch (info.commandID)
    {
        case CommandIDs::open_file:
            chooseFileToLoad();
            break;
    }
    return true;
}

void MainWindow::chooseFileToLoad()
{
    juce::FileChooser chooser("Select an audio file...", juce::File(), "*.wav");
    if (chooser.browseForFileToOpen()) { load(chooser.getResult()); }
}


void MainWindow::load(const juce::File& filepath)
{
    if (!m_ViewHandler.fileIsOpen(filepath))
    {
        juce::AudioFormatManager mgr;
        mgr.registerBasicFormats();
        std::unique_ptr<juce::AudioFormatReader> reader(mgr.createReaderFor(filepath));
        auto n_chans = reader->getChannelLayout().size();
        juce::AudioBuffer<float> data(n_chans, reader->lengthInSamples);
        reader->read(data.getArrayOfWritePointers(), n_chans, 0, reader->lengthInSamples);
        m_ViewHandler.addFile(filepath, data, reader->sampleRate);
    }

    juce::RecentlyOpenedFilesList recentFiles;
    recentFiles.addFile(filepath);
    getAppProperties().getUserSettings()->setValue(AppState::RECENT_FILES, recentFiles.toString());
}