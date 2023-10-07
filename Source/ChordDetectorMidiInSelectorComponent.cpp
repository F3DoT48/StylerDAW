/*
  ==============================================================================

    ChordDetectorMidiInSelectorComponent.cpp
    Created: 18 Aug 2023 6:48:47pm
    Author:  User

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ChordDetectorMidiInSelectorComponent.h"

using namespace styler_app;

//==============================================================================
ChordDetectorMidiInSelectorComponent::ChordDetectorMidiInSelectorComponent (te::Edit& edit)
    : mEdit { edit }
    , mMidiInputSelectorButton { "Midi in"}
{
    mMidiInputSelectorButton.onClick = [this]()
    {
        juce::PopupMenu midiInputMenu;

        auto* chordDetectorTrackPtr { te::getFirstAudioTrack (mEdit) };

        int itemId { 1 };

        for (auto inputDeviceInstance : mEdit.getAllInputDevices())
        {
            if (inputDeviceInstance->getInputDevice().getDeviceType() == te::InputDevice::physicalMidiDevice)
            {
                bool isTicked { inputDeviceInstance->isOnTargetTrack (*chordDetectorTrackPtr) };
                midiInputMenu.addItem (itemId++
                                     , inputDeviceInstance->getInputDevice().getName()
                                     , true
                                     , isTicked);
            }
        }

        if (itemId == 1)
        {
            midiInputMenu.addItem (-1
                                 , "No input devices found"
                                 , false
                                 , false);
        }

        int clickedItemId { midiInputMenu.show() };

        if (clickedItemId > 0)
        {
            itemId = 1;
            for (auto inputDeviceInstance : mEdit.getAllInputDevices())
            {
                if (inputDeviceInstance->getInputDevice().getDeviceType() == te::InputDevice::physicalMidiDevice)
                {
                    if (itemId == clickedItemId)
                    {
                        if (inputDeviceInstance->isOnTargetTrack (*chordDetectorTrackPtr))
                        {
                            inputDeviceInstance->removeTargetTrack (*chordDetectorTrackPtr);
                        }
                        else
                        {
                            inputDeviceInstance->setTargetTrack (*chordDetectorTrackPtr, 0, false);
                        }
                    }
                    itemId++;
                }
            }
        }
    };
    addAndMakeVisible (mMidiInputSelectorButton);
}

ChordDetectorMidiInSelectorComponent::~ChordDetectorMidiInSelectorComponent()
{
}

void ChordDetectorMidiInSelectorComponent::paint (juce::Graphics& g)
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
    g.drawText ("ChordDetectorMidiInSelectorComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void ChordDetectorMidiInSelectorComponent::resized()
{
    auto rec { getLocalBounds() };

    mMidiInputSelectorButton.setBounds (rec);
}
