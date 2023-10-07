/*
  ==============================================================================

    PRToolBarComponent.cpp
    Created: 3 May 2023 3:04:44pm
    Author:  User

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PRToolBarComponent.h"

using namespace styler_app;

//==============================================================================
PRToolBarComponent::PRToolBarComponent (ArrangementPattern::Ptr arrangementPattern
                                      , ArrangementSection::Ptr arrangementSection)
    : mState { "PRToolBarState", {{ArrangerIDs::channelIndex, 1}} }
    , mArrangementPattern { arrangementPattern }
    , mArrangementSection { arrangementSection }
    , mCurrentMidiChannel { mState.getProperty (ArrangerIDs::channelIndex) }
    , mNotePrototype { MidiNote::createNoteValueTree (0.0 // won't be used
                                                    , 1.0
                                                    , 0 // won't be used
                                                    , 100
                                                    , false
                                                    , NoteTranspositionRule::Type::ignore) }
    , mMidiChannelSelectorComponent { std::make_unique<MidiChannelSelectorComponent> (mState, mArrangementPattern) }
    , mSnapResolutionSelectorComponent { std::make_unique<SnapResolutionSelectorComponent> (mCurrentSnapOption) }
    , mNoteTranspositionRuleSelectorComponent { std::make_unique<NoteTranspositionRuleSelectorComponent>() }
    , mBarsInViewSelector { std::make_unique<BarsInViewSelectorComponent> (0.2f
                                                                         , static_cast<float> (mArrangementSection->getLengthInBars())
                                                                         , 0.1f
                                                                         , 1.0f) }
    , mKeysInViewSelector { std::make_unique<KeysInViewSelectorComponent> (7.0f
                                                                         , 75.0f
                                                                         , 1.0f
                                                                         , 14.0f) }
{
    addAndMakeVisible (mMidiChannelSelectorComponent.get());
    addAndMakeVisible (mSnapResolutionSelectorComponent.get());
    addAndMakeVisible (mBarsInViewSelector.get());
    addAndMakeVisible (mKeysInViewSelector.get());
}

PRToolBarComponent::~PRToolBarComponent()
{
}

void PRToolBarComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("PRToolBarComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void PRToolBarComponent::resized()
{
    auto rec { getLocalBounds()};

    mMidiChannelSelectorComponent->setBounds (rec.removeFromLeft (150));
    mSnapResolutionSelectorComponent->setBounds (rec.removeFromLeft (150));

    mBarsInViewSelector->setBounds (rec.removeFromRight (sToolWidthInPixels));
    mKeysInViewSelector->setBounds (rec.removeFromRight (sToolWidthInPixels));
}

te::MidiChannel PRToolBarComponent::getCurrentMidiChannel() const
{
    return te::MidiChannel { mState.getProperty (ArrangerIDs::channelIndex) };
}

double PRToolBarComponent::getSnapResolutionInBeats () const
{
    //const int ticksPerBeat { te::Edit::ticksPerQuarterNote };
    int beatsInBar { mArrangementPattern
                   ->getAssociatedAudioTrack()
                   ->edit.tempoSequence.getTimeSigs()[0]->numerator };

    switch (mCurrentSnapOption)
    {
        using enum SnapOption;

        case bar:
            return beatsInBar;
            break;
        case halfBar:
            return beatsInBar / 2.0;
            break;
        case beat:
            return 1.0;
            break;
        case halfBeat:
            return 1.0 / 2.0;
            break;
        case thirdBeat:
            return 1.0 / 3.0;
            break;
        case quarterBeat:
            return 1.0 / 4.0;
            break;
        case sixthBeat:
            return 1.0 / 6.0;
            break;
        case eighthBeat:
            return 1.0 / 8.0;
            break;
        case twelfthBeat:
            return 1.0 / 12.0;
            break;
        case sixteenthBeat:
            return 1.0 / 16.0;
            break;
        case smallest:
            return 1.0 / 96.0; // 10*te::ticksPerQuarterNote
            break;

        default:
            jassert ("Unknown SnapOption");
            return 1.0;
    }
}

float PRToolBarComponent::getNumBarsInView() const
{
    return mBarsInViewSelector->getCurrentNumBarsInView();
}

float PRToolBarComponent::getNumKeysInView() const
{
    return mKeysInViewSelector->getCurrentNumKeysInView();
}
