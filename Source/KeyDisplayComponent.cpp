/*
  ==============================================================================

    KeyDisplayComponent.cpp
    Created: 15 Aug 2023 10:21:20am
    Author:  User

  ==============================================================================
*/

#include <JuceHeader.h>
#include "KeyDisplayComponent.h"

using namespace styler_app;

//==============================================================================
KeyDisplayComponent::KeyDisplayComponent (Arrangement& arrangement)
    : mArrangement {arrangement}
    , mKeySymbolLabel{}
{
    mArrangement.mState.addListener (this);
    mKeySymbolLabel.setEditable (false);
    mKeySymbolLabel.setJustificationType (juce::Justification::centred);
    mKeySymbolLabel.setColour (juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible (&mKeySymbolLabel);
    triggerAsyncUpdate();
}

KeyDisplayComponent::~KeyDisplayComponent()
{
    mArrangement.mState.removeListener(this);
}

void KeyDisplayComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    /*g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("KeyDisplayComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text*/
}

void KeyDisplayComponent::resized()
{
    mKeySymbolLabel.setBounds (getLocalBounds());
}

void KeyDisplayComponent::valueTreePropertyChanged (juce::ValueTree& /* treeWhosePropertyHasChanged  */
                                                  , const juce::Identifier& /* property  */)
{
    triggerAsyncUpdate();
}

void KeyDisplayComponent::handleAsyncUpdate()
{
    switch (mArrangement.getPtrToController()->getKeyNoteIndex())
    {
        case 0:
            mKeySymbolLabel.setText ("C/Am"
                                   , juce::NotificationType::dontSendNotification);
            break;
        case 1:
            mKeySymbolLabel.setText ("C#(Db)/A#m(Bbm)"
                                   , juce::NotificationType::dontSendNotification);
            break;
        case 2:
            mKeySymbolLabel.setText ("D/Bm"
                                   , juce::NotificationType::dontSendNotification);
            break;
        case 3:
            mKeySymbolLabel.setText ("D#(Eb)/Cm"
                                   , juce::NotificationType::dontSendNotification);
            break;
        case 4:
            mKeySymbolLabel.setText ("E/C#m(Dbm)"
                                   , juce::NotificationType::dontSendNotification);
            break;
        case 5:
            mKeySymbolLabel.setText ("F/Dm"
                                   , juce::NotificationType::dontSendNotification);
            break;
        case 6:
            mKeySymbolLabel.setText ("F#(Gb)/D#m(Ebm)"
                                   , juce::NotificationType::dontSendNotification);
            break;
        case 7:
            mKeySymbolLabel.setText ("G/Em"
                                   , juce::NotificationType::dontSendNotification);
            break;
        case 8:
            mKeySymbolLabel.setText ("G#(Ab)/Fm"
                                   , juce::NotificationType::dontSendNotification);
            break;
        case 9:
            mKeySymbolLabel.setText ("A/F#m(Gbm)"
                                   , juce::NotificationType::dontSendNotification);
            break;
        case 10:
            mKeySymbolLabel.setText ("A#(Bb)/Gm"
                                   , juce::NotificationType::dontSendNotification);
            break;
        case 11:
            mKeySymbolLabel.setText ("B/G#m(Abm)"
                                   , juce::NotificationType::dontSendNotification);
            break;
        default:
            mKeySymbolLabel.setText ("error"
                                   , juce::NotificationType::dontSendNotification);
            break;
    }

    resized();
}

