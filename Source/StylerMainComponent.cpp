#include "StylerMainComponent.h"
#include "StylerTracktionEngineBehaviour.h"
#include "StylerUIBehaviour.h"
#include "SettingsComponent.h"


using namespace styler_app;

//==============================================================================
StylerMainComponent::StylerMainComponent()
    : mTracktionEngine{ juce::String (ProjectInfo::projectName)
                      , std::make_unique<StylerUIBehaviour>()
                      , std::make_unique<StylerTracktionEngineBehaviour>() }
    , mSelectionManager{ mTracktionEngine }
    , mMenuBar { std::make_unique<MainComponentMenuBar> (mCommandManager) }
    , mNewTrackButton { std::make_unique<juce::TextButton> ("Add new track") }
    , mNewArrangementSectionButton { std::make_unique<juce::TextButton> ("Add new section") }
    , mArrangementControlsAreaViewport { std::make_unique<juce::Viewport> ("ArrangementControlsAreaViewport") }
    , mEditComponentViewport { std::make_unique<juce::Viewport> ("EditComponentViewport") }
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
        createOrLoadEdit (directory.getNonexistentChildFile ("NewEdit", ".tracktionedit", false));
    }

    mSelectionManager.addChangeListener (this);

    mCommandManager.registerAllCommandsForTarget (this);
    mCommandManager.setFirstCommandTarget (this);

    addKeyListener (mCommandManager.getKeyMappings());
    setWantsKeyboardFocus (true);

    mEdit->getUndoManager().clearUndoHistory();

    addAndMakeVisible (mNewTrackButton.get());
    setupNewTrackButton();
    
    addAndMakeVisible (mNewArrangementSectionButton.get());
    setupNewArrangementSectionButton();

    addAndMakeVisible (mMenuBar.get());

    addAndMakeVisible (mArrangementControlsAreaViewport.get());
    addAndMakeVisible (mEditComponentViewport.get());

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

    auto rectForArrangementControlsAndButtons { rectangle.removeFromTop (ArrangementControlsArea::sHeightInPixels) };
    rectForArrangementControlsAndButtons.removeFromLeft (TrackComponentAttributes::inputsAreaWidthInPixels);

    auto recForButtons { rectForArrangementControlsAndButtons.removeFromRight (TrackComponentAttributes::mixerControlsParametersAreaWidthInPixels
                                                                             + juce::LookAndFeel::getDefaultLookAndFeel().getDefaultScrollbarWidth())
                                                             .removeFromLeft (TrackComponentAttributes::mixerControlsParametersAreaWidthInPixels) };
    
    mNewTrackButton->setBounds (recForButtons.removeFromTop (sAreaForButtonsHeight));
    mNewTrackButton->resized();

    mNewArrangementSectionButton->setBounds (recForButtons);
    mNewArrangementSectionButton->resized();

    rectForArrangementControlsAndButtons.removeFromRight (TrackComponentAttributes::pluginAreaWidthInPixels);

    if (mArrangementControlsArea != nullptr)
    {
        auto rectForArrangementControlsArea { rectForArrangementControlsAndButtons };

        rectForArrangementControlsArea.setRight (rectForArrangementControlsArea.getX()
                                               + (mArrangement->getAllSections().size()
                                                * ArrangementSectionControlsComponent::sWidthInPixels));

        rectForArrangementControlsArea.setBottom (rectForArrangementControlsArea.getBottom()
                                                - juce::LookAndFeel::getDefaultLookAndFeel().getDefaultScrollbarWidth());

        mArrangementControlsArea->setBounds (rectForArrangementControlsArea);
    }

    mArrangementControlsAreaViewport->setBounds (rectForArrangementControlsAndButtons);

    if (mEditComponent != nullptr)
    {
        auto rectForEditComponent { rectangle };

        rectForEditComponent.setBottom (rectForEditComponent.getY()
                                      + (mEdit->getTrackList().size() - 3) // 2 te + 1 audio track are hidden
                                      * (TrackComponentAttributes::minimumHeightInPixels 
                                       + TrackComponentAttributes::trackGapInPixels));
        
        rectForEditComponent.setRight (rectForEditComponent.getRight()
                                     - juce::LookAndFeel::getDefaultLookAndFeel().getDefaultScrollbarWidth());

        mEditComponent->setBounds (rectForEditComponent);
    }

    mEditComponentViewport->setBounds (rectangle.removeFromTop (7 // tracks
                                                              * (TrackComponentAttributes::minimumHeightInPixels
                                                               + TrackComponentAttributes::trackGapInPixels)));
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
        createOrLoadEdit ({});
        mEdit->getUndoManager().clearUndoHistory();
        break;
    case CommandIDs::fileOpen:
    {   
        juce::FileChooser fileChooser ("Open Edit", juce::File::getSpecialLocation (juce::File::userDocumentsDirectory), "*.tracktionedit");
        if (fileChooser.browseForFileToOpen())
        {
            createOrLoadEdit (fileChooser.getResult());
            mEdit->getUndoManager().clearUndoHistory();
        }        
    }
        break;
    case CommandIDs::fileSave:
        te::EditFileOperations (*mEdit).save (true, true, false);
        break;
    case CommandIDs::fileSaveAs:
        te::EditFileOperations (*mEdit).saveAs();
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

    getTopLevelComponent()->repaint();
    resized();
    return true;
}

void StylerMainComponent::createOrLoadEdit (juce::File editFile = {})
{
    if (editFile == juce::File())
    {
        juce::FileChooser fileChooser ("New Edit", juce::File::getSpecialLocation (juce::File::userDocumentsDirectory), "*.tracktionedit");
        if (fileChooser.browseForFileToSave (true))
            editFile = fileChooser.getResult();
        else
            return;
    }
    
    mSelectionManager.deselectAll();
    mEditComponent = nullptr;

    if (editFile.existsAsFile())
    {
        mEdit = te::loadEditFromFile (mTracktionEngine, editFile);
        mArrangement = std::make_unique<Arrangement>(*mEdit);
    }
    else
    {
        mEdit = te::createEmptyEdit (mTracktionEngine, editFile);
        mEdit->ensureNumberOfAudioTracks (2); // 1 hidden and 1 displayed tracks

        // add racks to audio  tracks
        auto audioTracks { getAudioTracks(*mEdit) };
        
        for (auto audioTrack : audioTracks)
        {
            auto rackTypePtr { mEdit->getRackList().addNewRack() };
            auto rackInstanceCreationInfo { te::RackInstance::create (*rackTypePtr) };
            auto rackFx { dynamic_cast<te::RackInstance*> (mEdit->getPluginCache().createNewPlugin (rackInstanceCreationInfo).get()) };
            audioTrack->pluginList.insertPlugin (*rackFx, 0, &mSelectionManager);
        }

        //mArrangement = Arrangement::createArrangementForNewEditAndAddToState (*mEdit);
        auto arrangementState { Arrangement::createArrangement (*mEdit) };
        mEdit->state.appendChild (arrangementState, nullptr);
        mArrangement = std::make_unique<Arrangement> (*mEdit);
    }

    mEdit->playInStopEnabled = true;
    auto& transport = mEdit->getTransport();
    transport.addChangeListener (this);

    te::EditFileOperations (*mEdit).save (true, true, false);

    enableAllInputs();

    mArrangementControlsArea = std::make_unique<ArrangementControlsArea> (*mArrangement);

    mArrangementControlsAreaViewport->setViewedComponent (mArrangementControlsArea.get(), false);

    mEditComponent = std::make_unique<EditComponent> (*mEdit, mSelectionManager, *mArrangement, *mArrangementControlsAreaViewport);

    mEditComponentViewport->setViewedComponent (mEditComponent.get(), false);

    setTopLevelComponentName();
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

void StylerMainComponent::setTopLevelComponentName()
{
    getTopLevelComponent()->setName (te::EditFileOperations (*mEdit).getEditFile().getFileName());
}

juce::Viewport* StylerMainComponent::getEditComponentViewport() const noexcept
{
    return mEditComponentViewport.get();
}

juce::Viewport* StylerMainComponent::getArrangementControlsAreaViewport() const noexcept
{
    return mArrangementControlsAreaViewport.get();
}


void StylerMainComponent::setupNewTrackButton()
{
    mNewTrackButton->onClick = [this]
    {
        auto audioTracks {getAudioTracks(*mEdit)};
        const int numGlobalTracks { 5 };
        if (audioTracks.size() < mEdit->engine.getEngineBehaviour().getEditLimits().maxNumTracks - numGlobalTracks)
        {
            mEdit->ensureNumberOfAudioTracks (audioTracks.size() + 1);
            auto addedTrack { mEdit->getTrackList()[mEdit->getTrackList().size() - 1] };
            auto rackTypePtr { mEdit->getRackList().addNewRack() };
            auto rackInstanceCreationInfo { te::RackInstance::create (*rackTypePtr) };
            auto rackFx { dynamic_cast<te::RackInstance*> (mEdit->getPluginCache().createNewPlugin(rackInstanceCreationInfo).get()) };
            addedTrack->pluginList.insertPlugin (*rackFx, 0, &mSelectionManager);
        }
    };
}

void StylerMainComponent::setupNewArrangementSectionButton()
{
    mNewArrangementSectionButton->onClick = [this]
    {
        mArrangement->addNewSection();
    };
}