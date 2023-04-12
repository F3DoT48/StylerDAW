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
{
    jassert (mState.isValid());
}

juce::ValueTree Arrangement::createArrangement (const te::Edit& edit)
{
    //auto valueTree { te::createValueTree (ArrangerIDs::arrangement) };

    //valueTree.appendChild (ArrangementSection::createSection (1, edit), nullptr);

    //juce::ValueTree valueTree (ArrangerIDs::arrangement);

    return juce::ValueTree ( ArrangerIDs::arrangement );
}

te::Edit& Arrangement::getEdit() noexcept
{
    return mEdit;
}

void Arrangement::addNewSection()
{
    if (mSections->size() < sMaxNumArrangementParts)
    {
        mState.appendChild (ArrangementSection::createSection (mEdit), &mEdit.getUndoManager());
    }
}

void Arrangement::removeSection (ArrangementSection* section)
{
    section->mState.getParent().removeChild (section->mState, &mEdit.getUndoManager());
}

ArrangementSectionList& Arrangement::getAllSections() noexcept
{
    return *mSections;
}