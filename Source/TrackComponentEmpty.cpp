/*
  ==============================================================================

    TrackComponentEmpty.cpp
    Created: 30 May 2022 7:28:51pm
    Author:  User

  ==============================================================================
*/

#include "TrackComponentEmpty.h"

using namespace styler_app;

TrackComponentEmpty::TrackComponentEmpty (EditViewState& state, te::Track::Ptr track)
    : TrackComponent (state, track)
{

}

TrackComponentEmpty::~TrackComponentEmpty()
{

}

void TrackComponentEmpty::paint (juce::Graphics& /* g */)
{

}

void TrackComponentEmpty::resized()
{

}