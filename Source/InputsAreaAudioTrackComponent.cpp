/*
  ==============================================================================

    InputsAreaComponent.cpp
    Created: 4 Jun 2022 10:36:47pm
    Author:  User

  ==============================================================================
*/

#include <JuceHeader.h>
#include "InputsAreaAudioTrackComponent.h"

using namespace styler_app;

//==============================================================================

InputsAreaAudioTrackComponent::InputsAreaAudioTrackComponent (EditViewState& editViewState, te::Track::Ptr track)
    : TrackComponent (editViewState, track)
    , mAudioTrack{ dynamic_cast<te::AudioTrack*> (track.get())}
    , mNameLabel {"Track name", mAudioTrack->getName()}
    , mDeleteTrackButton ("Delete track")
    , mMidiInputSelectorButton ("Midi in")
    , mAudioInputSelectorButton ("Audio in")
{
    mNameLabel.setEditable (true);
    mNameLabel.setJustificationType (juce::Justification::centred);
    mNameLabel.onTextChange = [this]()
    {
        mAudioTrack->setName (mNameLabel.getText());
    };

    addAndMakeVisible (mNameLabel);

    mDeleteTrackButton.onClick = [this]()
    {
        mEditViewState.mEdit.deleteTrack (getTrack().get());
    };

    mMidiInputSelectorButton.onClick = [this]()
    {
        juce::PopupMenu midiInputMenu;

        int itemId{ 1 };

        for (auto inputDeviceInstance : mAudioTrack->edit.getAllInputDevices())
        {
            if (inputDeviceInstance->getInputDevice().getDeviceType() == te::InputDevice::physicalMidiDevice)
            {
                bool isTicked {inputDeviceInstance->isOnTargetTrack (*mAudioTrack)};
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

        int clickedItemId{ midiInputMenu.show() };

        if (clickedItemId > 0)
        {
            itemId = 1;
            for (auto inputDeviceInstance : mAudioTrack->edit.getAllInputDevices())
            {
                if (inputDeviceInstance->getInputDevice().getDeviceType() == te::InputDevice::physicalMidiDevice)
                {
                    if (itemId == clickedItemId)
                    {
                        if (inputDeviceInstance->isOnTargetTrack (*mAudioTrack))
                        {
                            inputDeviceInstance->removeTargetTrack (*mAudioTrack);
                        }
                        else
                        {
                            inputDeviceInstance->setTargetTrack (*mAudioTrack, 0, false);
                        }
                    }
                    itemId++;
                }
            }
        }
    };

    mAudioInputSelectorButton.onClick = [this]()
    {
        juce::PopupMenu audioInputMenu;

        int itemId{ 1 };

        for (auto inputDeviceInstance : mAudioTrack->edit.getAllInputDevices())
        {
            if (inputDeviceInstance->getInputDevice().getDeviceType() == te::InputDevice::waveDevice)
            {
                bool isTicked {inputDeviceInstance->isOnTargetTrack (*mAudioTrack)};
                audioInputMenu.addItem (itemId++
                                      , inputDeviceInstance->getInputDevice().getName()
                                      , true
                                      , isTicked);
            }
        }

        if (itemId == 1)
        {
            audioInputMenu.addItem (-1
                                  , "No input devices found"
                                  , false
                                  , false);
        }            

        int clickedItemId{ audioInputMenu.show() };

        if (clickedItemId > 0)
        {
            itemId = 1;
            for (auto inputDeviceInstance : mAudioTrack->edit.getAllInputDevices())
            {
                if (inputDeviceInstance->getInputDevice().getDeviceType() == te::InputDevice::waveDevice)
                {
                    if (itemId == clickedItemId)
                    {
                        if (inputDeviceInstance->isOnTargetTrack (*mAudioTrack))
                        {
                            inputDeviceInstance->setRecordingEnabled (*mAudioTrack, false);
                            inputDeviceInstance->removeTargetTrack (*mAudioTrack);
                        }
                        else
                        {
                            inputDeviceInstance->setTargetTrack (*mAudioTrack, 0, false);
                            inputDeviceInstance->setRecordingEnabled (*mTrack, true);
                        }
                    }
                    itemId++;
                }
            }
        }
    };

    addAndMakeVisible (mDeleteTrackButton);
    addAndMakeVisible (mMidiInputSelectorButton);
    addAndMakeVisible (mAudioInputSelectorButton);

    mTrack->state.addListener (this);
    mInputsState = mTrack->edit.state.getChildWithName (te::IDs::INPUTDEVICES);
    mInputsState.addListener (this);

    valueTreePropertyChanged (mInputsState, te::IDs::targetIndex);
}

InputsAreaAudioTrackComponent::~InputsAreaAudioTrackComponent()
{
    mTrack->state.removeListener (this);
}

void InputsAreaAudioTrackComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText (mTrack->getName() + " InputsArea"
              , getLocalBounds()
              , juce::Justification::centred
              , true);
}

void InputsAreaAudioTrackComponent::resized()
{
    auto rectangle{ getLocalBounds()};

    mNameLabel.setBounds (rectangle.removeFromTop (20));
    mDeleteTrackButton.setBounds (rectangle.removeFromTop(20));
    mMidiInputSelectorButton.setBounds (rectangle.removeFromTop (20));
    mAudioInputSelectorButton.setBounds (rectangle.removeFromTop (20));
}

void InputsAreaAudioTrackComponent::valueTreeChanged()
{

}

void InputsAreaAudioTrackComponent::valueTreePropertyChanged (juce::ValueTree& valueTree
                                                            , const juce::Identifier& identifier)
{
    
}
