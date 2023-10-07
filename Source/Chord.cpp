/*
  ==============================================================================

    Chord.cpp
    Created: 20 Aug 2023 4:27:53pm
    Author:  User

  ==============================================================================
*/

#include "Chord.h"

using namespace styler_app;

Chord::Chord (int rootIndex, Type type)
    : mRootIndex { rootIndex }
    , mType { type }
{

}

void Chord::setRootIndex (int newRootIndex)
{
    mRootIndex = newRootIndex;
}

int Chord::getRootIndex() const
{
    return mRootIndex;
}

void Chord::setType (Chord::Type newType)
{
    mType = newType;
}

Chord::Type Chord::getType() const
{
    return mType;
}