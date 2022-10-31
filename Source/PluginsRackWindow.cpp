/*
  ==============================================================================

    PluginsRackWindow.cpp
    Created: 27 Oct 2022 4:06:25pm
    Author:  User

  ==============================================================================
*/

#include "PluginsRackWindow.h"

using namespace styler_app;

PluginsRackWindow::PluginsRackWindow (const juce::String& name)
    : juce::DocumentWindow (name
                          , juce::LookAndFeel::getDefaultLookAndFeel().findColour (juce::DocumentWindow::backgroundColourId)
                          , juce::DocumentWindow::TitleBarButtons::closeButton)
{
    setResizable (false, false);
}

PluginsRackWindow::~PluginsRackWindow()
{

}

void PluginsRackWindow::closeButtonPressed()
{
    setVisible (false);
}
