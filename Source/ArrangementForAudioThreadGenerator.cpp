/*
  ==============================================================================

    ArrangementForAudioThreadGenerator.cpp
    Created: 6 Sep 2023 5:21:25pm
    Author:  User

  ==============================================================================
*/

#include "ArrangementForAudioThreadGenerator.h"

using namespace styler_app;

ArrangementForAudioThreadGenerator::ArrangementForAudioThreadGenerator (Arrangement& arrangement)
    : mArrangement { arrangement }
    , mGarbagePool{}
    , mPreparedSectionList { nullptr }
{
    mArrangement.mState.addListener (this);

    deleteUnusedAndUpdateCurrent();
}

ArrangementForAudioThreadGenerator::~ArrangementForAudioThreadGenerator()
{
    mArrangement.mState.removeListener (this);
}

std::shared_ptr<const ArrangementSectionList> ArrangementForAudioThreadGenerator::getSectionListForAudioThread()
{
    return mPreparedSectionList.load();
}

void ArrangementForAudioThreadGenerator::deleteUnusedAndUpdateCurrent()
{
    mGarbagePool.cleanUp();

    const auto sectionListSharedPtr { std::make_shared<const ArrangementSectionList> (mArrangement.getEdit()
                                                                                    , mArrangement.mState.createCopy()
                                                                                    , nullptr) };

    mGarbagePool.addSharedInstance (sectionListSharedPtr);

    mPreparedSectionList.store (sectionListSharedPtr);
}

void ArrangementForAudioThreadGenerator::valueTreeChanged()
{
    triggerAsyncUpdate();
}

void ArrangementForAudioThreadGenerator::handleAsyncUpdate()
{
    deleteUnusedAndUpdateCurrent();
    DBG ("Cleaned up and generated a section list for audio thread.");
}