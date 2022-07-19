#include "StylerMainComponent.h"
#include "StylerTracktionEngineBehaviour.h"
#include "StylerUIBehaviour.h"
#include "SettingsComponent.h"


using namespace styler_app;

//==============================================================================
StylerMainComponent::StylerMainComponent()
    : mTracktionEngine{ juce::String (ProjectInfo::projectName)
                      , std::make_unique<styler_app::StylerUIBehaviour>()
                      , std::make_unique<styler_app::StylerTracktionEngineBehaviour>() }
    , mSelectionManager{ mTracktionEngine }
    , mEditComponentViewport{ std::make_unique<juce::Viewport> ("EditComponentViewport") }
    , mMenuBar{ std::make_unique<styler_app::MainComponentMenuBar> (mCommandManager) }
{
    auto directory = juce::File::getSpecialLocation (juce::File::tempDirectory).getChildFile ("Styler");
    directory.createDirectory();

    auto editFile = findRecentEdit (directory);
    if (editFile.existsAsFile())
    {
        createOrLoadEdit (editFile);
    }
    else
    {
        createOrLoadEdit (directory.getNonexistentChildFile ("NewEdit", ".edit", false));
    }

    mSelectionManager.addChangeListener (this);

    mCommandManager.registerAllCommandsForTarget (this);
    mCommandManager.setFirstCommandTarget (this);

    addKeyListener (mCommandManager.getKeyMappings());
    setWantsKeyboardFocus (true);

    mEdit->getUndoManager().clearUndoHistory();

    mEditComponentViewport->setViewedComponent (mEditComponent.get());

    addAndMakeVisible (mEditComponentViewport.get());

    addAndMakeVisible (mMenuBar.get());

    setSize (1024, 768);
}

StylerMainComponent::~StylerMainComponent()
{
    mCommandManager.setFirstCommandTarget (nullptr);

    te::EditFileOperations (*mEdit).save (true, true, false);
    mTracktionEngine.getTemporaryFileManager().getTempDirectory().deleteRecursively();
}

//==============================================================================
void StylerMainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setFont (juce::Font (16.0f));
    g.setColour (juce::Colours::white);
    g.drawText ("StylerMainComponent", getLocalBounds(), juce::Justification::centred, true);
}

void StylerMainComponent::resized()
{
    auto rectangle = getLocalBounds();

    mMenuBar->setBounds (rectangle.removeFromTop (juce::LookAndFeel::getDefaultLookAndFeel().getDefaultMenuBarHeight()));

    if (mEditComponent != nullptr)
    {
        rectangle.setBottom (rectangle.getY()
                           + mEdit->getTrackList().size()
                           * (TrackComponentAttributes::minimumHeightInPixels 
                            + TrackComponentAttributes::trackGapInPixels)
                           + TrackComponentAttributes::newTrackButtonHeight);
        auto rectForEditComponent = rectangle;
        
        rectForEditComponent.setRight (rectForEditComponent.getRight()
                                     - juce::LookAndFeel::getDefaultLookAndFeel().getDefaultScrollbarWidth());

        mEditComponent->setBounds (rectForEditComponent);
        mEditComponentViewport->setBounds (rectangle.removeFromTop (500));
    }
}

juce::ApplicationCommandTarget* styler_app::StylerMainComponent::getNextCommandTarget()
{
    return nullptr;
}

void styler_app::StylerMainComponent::getAllCommands (juce::Array<juce::CommandID>& commands)
{
    juce::Array<juce::CommandID> tmpCommands{ CommandIDs::fileNew
                                            , CommandIDs::fileOpen
                                            , CommandIDs::fileSave
                                            , CommandIDs::fileSaveAs
                                            , CommandIDs::fileQuit
                                            , CommandIDs::editUndo
                                            , CommandIDs::editRedo
                                            , CommandIDs::editDeleteSelected
                                            , CommandIDs::optionsSettings
                                            , CommandIDs::helpHelp
                                            , CommandIDs::helpAbout };

    commands.addArray (tmpCommands);
}

void styler_app::StylerMainComponent::getCommandInfo (juce::CommandID commandID, juce::ApplicationCommandInfo& result)
{
    switch (commandID)
    {
    case CommandIDs::fileNew:
        result.setInfo ("New", "Create a new project", "MenuBarCommand", 0);
        result.addDefaultKeypress ('n', juce::ModifierKeys::ctrlModifier);
        break;
    case CommandIDs::fileOpen:
        result.setInfo ("Open", "Open existing project", "MenuBarCommand", 0);
        result.addDefaultKeypress ('o', juce::ModifierKeys::ctrlModifier);
        break;
    case CommandIDs::fileSave:
        result.setInfo ("Save", "Save current project", "MenuBarCommand", 0);
        result.addDefaultKeypress ('s', juce::ModifierKeys::ctrlModifier);
        break;
    case CommandIDs::fileSaveAs:
        result.setInfo ("Save as", "Save current project", "MenuBarCommand", 0);
        result.addDefaultKeypress ('s', juce::ModifierKeys::ctrlModifier | juce::ModifierKeys::shiftModifier);
        break;
    case CommandIDs::fileQuit:
        result.setInfo ("Quit", "Quit the app", "MenuBarCommand", 0);
        result.addDefaultKeypress ('q', juce::ModifierKeys::ctrlModifier);
        break;
    case CommandIDs::editUndo:
        result.setInfo ("Undo", "Undo the last operation", "MenuBarCommand", 0);
        result.addDefaultKeypress ('z', juce::ModifierKeys::ctrlModifier);
        break;
    case CommandIDs::editRedo:
        result.setInfo ("Redo", "Redo the last operation", "MenuBarCommand", 0);
        result.addDefaultKeypress ('z', juce::ModifierKeys::ctrlModifier | juce::ModifierKeys::shiftModifier);
        break;
    case CommandIDs::editDeleteSelected:
        result.setInfo ("Delete selected", "Delete the selected item", "MenuBarCommand", 0);
        result.addDefaultKeypress (juce::KeyPress::deleteKey, juce::ModifierKeys::noModifiers);
        break;
    case CommandIDs::optionsSettings:
        result.setInfo ("Settings", "Open the Settings window", "MenuBarCommand", 0);
        break;
    case CommandIDs::helpHelp:
        result.setInfo ("Help", "Open the Help window", "MenuBarCommand", 0);
        break;
    case CommandIDs::helpAbout:
        result.setInfo ("About", "Open the About window", "MenuBarCommand", 0);
        break;
    default:
        break;
    }
}

bool styler_app::StylerMainComponent::perform (const InvocationInfo& invocationInfo)
{
    switch (invocationInfo.commandID)
    {
    case CommandIDs::fileNew:
        // to be implemented
        break;
    case CommandIDs::fileOpen:
        // to be implemented
        break;
    case CommandIDs::fileSave:
        te::EditFileOperations (*mEdit).save (true, true, false);
        break;
    case CommandIDs::fileSaveAs:
        // to be implemented
        break;
    case CommandIDs::fileQuit:
        juce::JUCEApplication::getInstance()->systemRequestedQuit();
        break;
    case CommandIDs::editUndo:
        mEdit->getUndoManager().undo();
        break;
    case CommandIDs::editRedo:
        mEdit->getUndoManager().redo();
        break;
    case CommandIDs::editDeleteSelected:
        // to be implemented
        break;
    case CommandIDs::optionsSettings:
    {
        juce::DialogWindow::LaunchOptions settingsWindowLaunchOptions;
        settingsWindowLaunchOptions.dialogTitle = "Settings";
        settingsWindowLaunchOptions.dialogBackgroundColour = getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId);
        settingsWindowLaunchOptions.escapeKeyTriggersCloseButton = false;
        settingsWindowLaunchOptions.resizable = true;
        settingsWindowLaunchOptions.useBottomRightCornerResizer = true;
                
        auto settingsComponent { new SettingsComponent (mTracktionEngine) };

        settingsComponent->setSize (800, 600);
        settingsWindowLaunchOptions.content.setOwned (settingsComponent);
        settingsWindowLaunchOptions.launchAsync();
    }
        break;
    case CommandIDs::helpHelp:
        // to be implemented
        break;
    case CommandIDs::helpAbout:
        // to be implemented
        break;
    default:
        return false;
    }

    repaint();
    return true;
}

void StylerMainComponent::createOrLoadEdit (juce::File editFile = {})
{
    if (editFile == juce::File())
    {
        juce::FileChooser fileChooser ("New Edit", juce::File::getSpecialLocation (juce::File::userDocumentsDirectory), "*.edit");
        if (fileChooser.browseForFileToSave (true))
            editFile = fileChooser.getResult();
        else
            return;
    }

    mSelectionManager.deselectAll();
    mEditComponent = nullptr;

    if (editFile.existsAsFile())
        mEdit = te::loadEditFromFile (mTracktionEngine, editFile);
    else
        mEdit = te::createEmptyEdit (mTracktionEngine, editFile);

    mEdit->playInStopEnabled = true;
    auto& transport = mEdit->getTransport();
    transport.addChangeListener (this);

    te::EditFileOperations (*mEdit).save (true, true, false);

    enableAllInputs();

    mEditComponent = std::make_unique<EditComponent> (*mEdit, mSelectionManager);

    addChildComponent (mEditComponent.get());
}

void StylerMainComponent::enableAllInputs()
{
    auto& deviceManager = mTracktionEngine.getDeviceManager();

    for (int i = 0; i < deviceManager.getNumMidiInDevices(); i++)
    {
        if (auto midiInputDevice = deviceManager.getMidiInDevice (i))
        {
            midiInputDevice->setEndToEndEnabled (true);
            midiInputDevice->setEnabled (true);
        }
    }

    for (int i = 0; i < deviceManager.getNumWaveInDevices(); i++)
    {
        if (auto waveInputDevice = deviceManager.getWaveInDevice (i))
        {
            waveInputDevice->setStereoPair (false);
        }
    }

    for (int i = 0; i < deviceManager.getNumWaveInDevices(); i++)
    {
        if (auto waveInputDevice = deviceManager.getWaveInDevice (i))
        {
            waveInputDevice->setEndToEnd (true);
            waveInputDevice->setEnabled (true);
        }
    }

    mEdit->getTransport().ensureContextAllocated();
}

void StylerMainComponent::changeListenerCallback (juce::ChangeBroadcaster* /* source */)
{

}

