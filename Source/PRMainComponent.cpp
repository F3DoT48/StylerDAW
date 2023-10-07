/*
  ==============================================================================

    PRMainComponent.cpp
    Created: 3 May 2023 3:16:39pm
    Author:  User

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PRMainComponent.h"

using namespace styler_app;

//==============================================================================
PRMainComponent::PRMainComponent (ArrangementPattern::Ptr arrangementPattern
                                , ArrangementSection::Ptr arrangementSection)
    : mArrangementPattern { arrangementPattern }
    , mArrangementSection { arrangementSection }
    , mToolBar { std::make_unique<PRToolBarComponent> (mArrangementPattern, mArrangementSection) }
    , mNoteEditor { std::make_unique<PRNoteEditorComponent> (mArrangementPattern, mArrangementSection, *mToolBar) }
    , mEventEditor { std::make_unique<PREventEditorComponent>() }
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.


    addAndMakeVisible (mToolBar.get());
    addAndMakeVisible (mNoteEditor.get());
    addAndMakeVisible (mEventEditor.get());
}

PRMainComponent::~PRMainComponent()
{
}

void PRMainComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("PRMainComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void PRMainComponent::resized()
{
    DBG ("PRMainComponent resized");
    auto rec { getLocalBounds() };

    mToolBar->setBounds (rec.removeFromTop (sToolBarHeightInPixels));
    mEventEditor->setBounds (rec.removeFromBottom (sEventEditorDefaultHeightInPixels));
    mNoteEditor->setBounds (rec);

    mToolBar->resized();
    mEventEditor->resized();
    mNoteEditor->resized();
}
