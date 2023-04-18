/*
  ==============================================================================

    StylerTracktionEngineBehaviour.cpp
    Created: 29 Apr 2022 12:50:53pm
    Author:  User

  ==============================================================================
*/

#include "StylerTracktionEngineBehaviour.h"

using namespace styler_app;

StylerTracktionEngineBehaviour::StylerTracktionEngineBehaviour()
    : mEditLimits{ .maxNumTracks{ 5 + 1 + 16 } // 5 te top level tracks (master, chord, tempo, marker, arranger) + 1 hidden track + 16 user audio tracks
                 , .maxClipsInTrack{ 20 } // a proxy to arrangement parts ?
                 , .maxPluginsOnClip{ 0 } // what is a plugin on a clip?
                 , .maxPluginsOnTrack{ 1 + 1 + 3 } // 1 rack with arrangement part players + 1 rack with user plugins + stereo processor + volume/pan + meters 
                 , .maxNumMasterPlugins{ 1 + 3 } } // 1 rack with user plugins + stereo processor + volume/pan + meters 
{
    
}

te::EditLimits StylerTracktionEngineBehaviour::getEditLimits()
{
    return mEditLimits;
}