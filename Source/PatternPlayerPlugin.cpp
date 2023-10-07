/*
  ==============================================================================

    PatternPlayerPlugin.cpp
    Created: 10 Aug 2023 2:04:51pm
    Author:  User

  ==============================================================================
*/

#include "PatternPlayerPlugin.h"

using namespace styler_app;

PatternPlayerPlugin::PatternPlayerPlugin (te::PluginCreationInfo creationInfo)
    : te::Plugin { creationInfo }
    , mArrangementController { nullptr }
    , mGenerator { nullptr }
    , mOwnerTrackIndex { 0 }
    , mBeatsPerSecond { 120.0 }
    , mBeatsSinceSectionStart { 0.0 }
{
    DBG ("PatternPlayerPlugin created");
}

PatternPlayerPlugin::~PatternPlayerPlugin()
{
    notifyListenersOfDeletion();
}

juce::String PatternPlayerPlugin::getName() const
{
    return "PatternPlayerPlugin";
}

const char* PatternPlayerPlugin::xmlTypeName ("PatternPlayerPlugin");

juce::String PatternPlayerPlugin::getPluginType()
{
    return xmlTypeName;
}

void PatternPlayerPlugin::initialise (const te::PluginInitialisationInfo&)
{

}

void PatternPlayerPlugin::deinitialise()
{

}

void PatternPlayerPlugin::prepareForNextBlock (tracktion::TimePosition editTime)
{
    te::Track* ownerTrack { getOwnerTrack() };

    if (ownerTrack)
    {
        mBeatsPerSecond = ownerTrack->edit.tempoSequence.getTempos()[0]->bpm / 60.0;
        const double editTimeInSeconds { editTime.inSeconds() };

        const int sectionIndex { mArrangementController.load()->getCurrentSectionIndex() };
        auto sectionList { mGenerator.load()->getSectionListForAudioThread() };

        if (editTimeInSeconds == 0.0)
        {
            mBeatsSinceSectionStart = 0.0;
        }
        else if (editTimeInSeconds > 0.0
              && sectionIndex < sectionList->size())
        { 
            const int sectionLengthInBeats { (*sectionList)[sectionIndex]->getLengthInBars()
                                           * ownerTrack->edit.tempoSequence.getTimeSigs()[0]->numerator };

            const double beatsSincePlayStart { editTimeInSeconds * mBeatsPerSecond };

            mBeatsSinceSectionStart = beatsSincePlayStart
                                    - static_cast<int> (beatsSincePlayStart / sectionLengthInBeats)
                                    * sectionLengthInBeats;
        }
    }
}


void PatternPlayerPlugin::applyToBuffer(const te::PluginRenderContext& context)
{
    
    const int sectionIndex { mArrangementController.load()->getCurrentSectionIndex() };
    auto sectionList { mGenerator.load()->getSectionListForAudioThread() };

    te::Track* ownerTrack { getOwnerTrack() };

    /*DBG ("Track : " << mOwnerTrackIndex.load()
      << " " << static_cast<int> (context.isPlaying)
      << " " << static_cast<int> (sectionIndex < sectionList->size())
      << " " << static_cast<int> (static_cast<bool> (ownerTrack)));*/

    if (context.isPlaying
     && sectionIndex < sectionList->size()
     && ownerTrack)
    {
        const auto& patternList { (*sectionList)[sectionIndex]->getAllPatterns() };


        juce::Range<double> editTimeInBeats { mBeatsSinceSectionStart
                                            , mBeatsSinceSectionStart
                                            + context.editTime.getLength().inSeconds() * mBeatsPerSecond};

        const int sectionLengthInBeats { (*sectionList)[sectionIndex]->getLengthInBars()
                                       * ownerTrack->edit.tempoSequence.getTimeSigs()[0]->numerator };

        const int patternIndex { mOwnerTrackIndex.load() - 1 };

        if (patternIndex < patternList.size())
        {
            const auto& pattern { patternList[patternIndex] };

            auto* midiBuffer { context.bufferForMidiMessages };

            for (int channel { 1 }; channel <= 16; ++channel)
            {
                const auto& notes { pattern->getMidiSequenceForChannel (channel)->getNotes() };
                
                for (const auto& note : notes)
                {
                    DBG ("Track : " << mOwnerTrackIndex.load() 
                      << " " << mBeatsSinceSectionStart
                      << " " << editTimeInBeats.getStart()
                      << " " << editTimeInBeats.getEnd()
                      << " " << note->getStartBeat());

                    if (const double noteStartInBeats {note->getStartBeat()}; 
                        editTimeInBeats.contains (noteStartInBeats)) // handle begin and mid-loop note-on messages
                    {
                        const double offsetInSeconds { (noteStartInBeats - mBeatsSinceSectionStart)
                                                      / mBeatsPerSecond /* * sampleRate */ };

                        DBG ("Track : " << mOwnerTrackIndex.load() << " On offset: " << offsetInSeconds);

                        midiBuffer->addMidiMessage (juce::MidiMessage::noteOn (channel
                                                                             , note->getNoteIndex()
                                                                             , static_cast<juce::uint8> (note->getVelocity()))
                                                  , offsetInSeconds
                                                  , te::MidiMessageArray::notMPE);
                    }
                    if (const double noteStartInBeats { note->getStartBeat() + sectionLengthInBeats };
                        editTimeInBeats.contains (noteStartInBeats)) // handle past-end-loop note-on messages
                    {
                        const double offsetInSeconds { (noteStartInBeats - mBeatsSinceSectionStart)
                                                      / mBeatsPerSecond /* * sampleRate */ };

                        DBG ("Track : " << mOwnerTrackIndex.load() << " On offset: " << offsetInSeconds);

                        midiBuffer->addMidiMessage (juce::MidiMessage::noteOn (channel
                                                                               , note->getNoteIndex()
                                                                               , static_cast<juce::uint8> (note->getVelocity()))
                                                  , offsetInSeconds
                                                  , te::MidiMessageArray::notMPE);
                    }
                    if (const double noteEndInBeats { note->getStartBeat() + note->getLengthInBeats() };
                        editTimeInBeats.contains (noteEndInBeats)) // handle begin and mid-loop note-off messages
                    {
                        const double offsetInSeconds { (noteEndInBeats - mBeatsSinceSectionStart)
                                                     / mBeatsPerSecond /* * sampleRate */ };

                        DBG ("Track : " << mOwnerTrackIndex.load() << " Off offset: " << offsetInSeconds);

                        midiBuffer->addMidiMessage (juce::MidiMessage::noteOff (channel
                                                                              , note->getNoteIndex())
                                                  , offsetInSeconds
                                                  , te::MidiMessageArray::notMPE);
                    }
                    if (const double noteEndInBeats { note->getStartBeat() + note->getLengthInBeats() + sectionLengthInBeats };
                        editTimeInBeats.contains (noteEndInBeats)) // handle past-end-loop note-off messages
                    {
                        const double offsetInSeconds { (noteEndInBeats - mBeatsSinceSectionStart)
                                                     / mBeatsPerSecond /* * sampleRate */ };

                        DBG ("Track : " << mOwnerTrackIndex.load() << " Off offset: " << offsetInSeconds);

                        midiBuffer->addMidiMessage (juce::MidiMessage::noteOff (channel
                                                                                , note->getNoteIndex())
                                                  , offsetInSeconds
                                                  , te::MidiMessageArray::notMPE);
                    }
                }
            }
            
            midiBuffer->sortByTimestamp(); 
            //DBG ("Track : " << mOwnerTrackIndex.load() << " Total notes count : " << midiBuffer->size());
        }
    }
    
}

double PatternPlayerPlugin::getLatencySeconds()
{
    return 0.0;
}

int PatternPlayerPlugin::getNumOutputChannelsGivenInputs (int numInputs)
{
    return juce::jmax (2, numInputs);
}

void PatternPlayerPlugin::getChannelNames (juce::StringArray*, juce::StringArray*)
{

}

bool PatternPlayerPlugin::canBeAddedToClip()
{
    return false;
}

bool PatternPlayerPlugin::needsConstantBufferSize()
{
    return false;
}

juce::String PatternPlayerPlugin::getSelectableDescription()
{
    return "PatternPlayerPlugin";
}

void PatternPlayerPlugin::setArrangementController (ArrangementController* controllerToUse)
{
    mArrangementController.store (controllerToUse);
}


void PatternPlayerPlugin::setArrangementForAudioThreadGenerator (ArrangementForAudioThreadGenerator* generatorToUse)
{
    mGenerator.store (generatorToUse);
}

void PatternPlayerPlugin::setOwnerTrackIndex (int newIndex)
{
    mOwnerTrackIndex.store (newIndex);
}