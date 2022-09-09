/*
  ==============================================================================

    MainComponentMenuBar.cpp
    Created: 5 May 2022 12:05:20pm
    Author:  User

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MainComponentMenuBar.h"
#include "CommandIDs.h"

using namespace styler_app;

//==============================================================================
MainComponentMenuBar::MainComponentMenuBar (juce::ApplicationCommandManager& commandManager)
    : mCommandManager{ commandManager }
    , mPopupMenu{ std::make_unique<juce::MenuBarComponent> (this)}
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    setApplicationCommandManagerToWatch (&mCommandManager);

    addAndMakeVisible (mPopupMenu.get());
}

MainComponentMenuBar::~MainComponentMenuBar()
{
}

void MainComponentMenuBar::paint (juce::Graphics& /* g */)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    /*g.fillAll (getLookAndFeel ().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("MainComponentMenuBar", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
                */
}

void MainComponentMenuBar::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    mPopupMenu->setBounds (getLocalBounds());
}

juce::StringArray styler_app::MainComponentMenuBar::getMenuBarNames ()
{
    return { "File", "Edit", "Options", "Help" };
}

juce::PopupMenu styler_app::MainComponentMenuBar::getMenuForIndex (int topLevelMenuIndex, const juce::String& /* menuName */)
{
    juce::PopupMenu popupMenu;

    switch (topLevelMenuIndex)
    {
    case 0: // File menu
        popupMenu.addCommandItem (&mCommandManager, CommandIDs::fileNew);
        popupMenu.addCommandItem (&mCommandManager, CommandIDs::fileOpen);
        popupMenu.addCommandItem (&mCommandManager, CommandIDs::fileSave);
        popupMenu.addCommandItem (&mCommandManager, CommandIDs::fileSaveAs);
        popupMenu.addCommandItem (&mCommandManager, CommandIDs::fileQuit);
        break;
    case 1: // Edit menu
        popupMenu.addCommandItem (&mCommandManager, CommandIDs::editUndo);
        popupMenu.addCommandItem (&mCommandManager, CommandIDs::editRedo);
        popupMenu.addCommandItem (&mCommandManager, CommandIDs::editDeleteSelected);
        break;
    case 2: // Options menu
        popupMenu.addCommandItem (&mCommandManager, CommandIDs::optionsSettings);
        break;
    case 3: // Help menu
        popupMenu.addCommandItem (&mCommandManager, CommandIDs::helpHelp);
        popupMenu.addCommandItem (&mCommandManager, CommandIDs::helpAbout);
    }

    return popupMenu;
}

void styler_app::MainComponentMenuBar::menuItemSelected (int /* menuItemID */, int /* topLevelMenuIndex */)
{
    // do nothing
}
