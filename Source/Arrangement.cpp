/*
  ==============================================================================

    Arrangement.cpp
    Created: 20 Nov 2022 6:20:04pm
    Author:  User

  ==============================================================================
*/

#include "Arrangement.h"
#include "ArrangerIDs.h"

using namespace styler_app;

Arrangement::Arrangement (te::Edit& edit)
    : mState { edit.state.getChildWithName (ArrangerIDs::arrangement) }
    , mEdit { edit }
    , mSections {std::make_unique<ArrangementSectionList> (mEdit
                                                         , mState
                                                         , nullptr)}
    , mController {mState, nullptr}
{
    jassert (mState.isValid());
}

juce::ValueTree Arrangement::createArrangement (const te::Edit& edit)
{
    return te::createValueTree (ArrangerIDs::arrangement 
                              , ArrangerIDs::splitNoteIndex, 30 // F# after two octaves
                              , ArrangerIDs::keyNoteIndex, 0); // C/Am
}

te::Edit& Arrangement::getEdit()
{
    return mEdit;
}

void Arrangement::addNewSection()
{
    if (mSections->size() < sMaxNumArrangementSections)
    {
        mState.appendChild (ArrangementSection::createSection (mEdit), &mEdit.getUndoManager());
    }
}

void Arrangement::removeSection (ArrangementSection* section)
{
    section->mState.getParent().removeChild (section->mState, &mEdit.getUndoManager());
}

ArrangementSectionList& Arrangement::getAllSections()
{
    jassert (mSections != nullptr);
    return *mSections;
}

ArrangementController* Arrangement::getPtrToController()
{
    return &mController;
}