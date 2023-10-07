/*
  ==============================================================================

    ArrangementControllerPlugin.cpp
    Created: 10 Aug 2023 2:05:13pm
    Author:  User

  ==============================================================================
*/

#include "ArrangementControllerPlugin.h"
#include "Arrangement.h"

using namespace styler_app;

ArrangementControllerPlugin::ArrangementControllerPlugin (te::PluginCreationInfo creationInfo)
    : te::Plugin { creationInfo }
    , mArrangementController { nullptr }
    , mChordDetector{}
{
    DBG ("ControllerPlugin created");
}

ArrangementControllerPlugin::~ArrangementControllerPlugin()
{
    notifyListenersOfDeletion();
}

juce::String ArrangementControllerPlugin::getName() const
{
    return "ArrangementControllerPlugin";
}

const char* ArrangementControllerPlugin::xmlTypeName ("ArrangementControllerPlugin");

juce::String ArrangementControllerPlugin::getPluginType()
{
    return xmlTypeName;
}

void ArrangementControllerPlugin::initialise (const te::PluginInitialisationInfo&)
{

}

void ArrangementControllerPlugin::deinitialise()
{

}

void ArrangementControllerPlugin::applyToBuffer (const te::PluginRenderContext& context)
{
    if (mArrangementController.load())
    {
        for (auto& message : *context.bufferForMidiMessages)
        {
            auto currChannel { message.getChannel() };
            auto currNoteIndex { message.getNoteNumber() };

            DBG (currNoteIndex);

            if (currChannel == 16)
            {
                if (currNoteIndex >= 0 && currNoteIndex <= 11)
                {
                    mArrangementController.load()->setKeyNoteIndex (currNoteIndex);
                }
                else if (currNoteIndex > 11 && currNoteIndex < 11 + Arrangement::sMaxNumArrangementSections)
                {
                    mArrangementController.load()->setCurrentSectionIndex (currNoteIndex - 11);
                }
            }
            else if (currNoteIndex <= mArrangementController.load()->getSplitNoteIndex())
            {
                if (message.isNoteOn())
                    mChordDetector.mChordNotesTracker.noteOn (currNoteIndex);
                else if (message.isNoteOff())
                    mChordDetector.mChordNotesTracker.noteOff (currNoteIndex);
            }
        }
    }

    //DBG (mChordDetector.getCurrentChord().getRootIndex() << " " << mChordDetector.mChordNotesTracker.getNotesAsBitSequence().to_string());
    mArrangementController.load()->setLastDetectedChord (mChordDetector.getCurrentChord());

}

double ArrangementControllerPlugin::getLatencySeconds()
{
    return 0.0;
}

int ArrangementControllerPlugin::getNumOutputChannelsGivenInputs (int)
{
    return 0;
}

void ArrangementControllerPlugin::getChannelNames (juce::StringArray*, juce::StringArray*)
{

}

bool ArrangementControllerPlugin::takesAudioInput()
{
    return false;
}

bool ArrangementControllerPlugin::canBeAddedToClip()
{
    return false;
}

bool ArrangementControllerPlugin::needsConstantBufferSize()
{
    return false;
}

juce::String ArrangementControllerPlugin::getSelectableDescription()
{
    return "ArrangementControllerPlugin";
}

void ArrangementControllerPlugin::setArrangementController (ArrangementController* controllerToUse)
{
    mArrangementController.store (controllerToUse);
}