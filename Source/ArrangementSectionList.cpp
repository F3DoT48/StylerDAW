/*
  ==============================================================================

    ArrangementSectionList.cpp
    Created: 5 Apr 2023 8:15:40pm
    Author:  User

  ==============================================================================
*/

#include "ArrangementSectionList.h"
#include "ArrangerIDs.h"

using namespace styler_app;

ArrangementSectionList::ArrangementSectionList (te::Edit& edit
                                              , const juce::ValueTree& parentState
                                              , juce::UndoManager* undoManager)
    : te::ValueTreeObjectList<ArrangementSection> {parentState}
    , mEdit {edit}
{
    rebuildObjects();
}

ArrangementSectionList::~ArrangementSectionList()
{
    freeObjects();
}

bool ArrangementSectionList::isSuitableType (const juce::ValueTree& arrSectionState) const
{
    return arrSectionState.hasType (ArrangerIDs::arrangementSection);
}

ArrangementSection* ArrangementSectionList::createNewObject (const juce::ValueTree& arrSectionState)
{
    ArrangementSection::Ptr arrSectionPtr {new ArrangementSection (mEdit, arrSectionState, nullptr)};
    arrSectionPtr->incReferenceCount();

    return arrSectionPtr.get();
}

void ArrangementSectionList::deleteObject (ArrangementSection* arrSectionPtr)
{
    jassert (arrSectionPtr != nullptr);

    arrSectionPtr->decReferenceCount();
}

void ArrangementSectionList::newObjectAdded (ArrangementSection*)
{

}

void ArrangementSectionList::objectRemoved (ArrangementSection*)
{

}

void ArrangementSectionList::objectOrderChanged()
{

}