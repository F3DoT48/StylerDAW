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
    : mEditLimits{ .maxNumTracks{ 32 }
                 , .maxClipsInTrack{ 16 }
                 , .maxPluginsOnClip{ 0 }
                 , .maxPluginsOnTrack{ 8 }
                 , .maxNumMasterPlugins{ 8 } }
{
    
}

te::EditLimits StylerTracktionEngineBehaviour::getEditLimits()
{
    return mEditLimits;
}