/*
  ==============================================================================

    PianoRollWindow.cpp
    Created: 3 May 2023 1:40:09pm
    Author:  User

  ==============================================================================
*/

#include "PianoRollWindow.h"

using namespace styler_app;

PianoRollWindow::PianoRollWindow (const juce::String& name)
    : juce::DocumentWindow (name
                          , juce::LookAndFeel::getDefaultLookAndFeel().findColour (juce::DocumentWindow::backgroundColourId)
                          , juce::DocumentWindow::TitleBarButtons::maximiseButton
                          | juce::DocumentWindow::TitleBarButtons::closeButton)
{
    setResizable (true, true);
    setResizeLimits (800
                   , 600
                   , getConstrainer()->getMaximumWidth()
                   , getConstrainer()->getMaximumHeight());
}

PianoRollWindow::~PianoRollWindow()
{

}

void PianoRollWindow::closeButtonPressed()
{
    setVisible (false);
}