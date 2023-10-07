/*
  ==============================================================================

    TempoSelectorComponent.cpp
    Created: 19 Aug 2023 5:28:55pm
    Author:  User

  ==============================================================================
*/

#include <JuceHeader.h>
#include "TempoSelectorComponent.h"

using namespace styler_app;

TempoSelectorComponent::TempoSelectorComponent (te::TempoSequence& tempoSequence)
    : ScrollAndDragSelectorComponent<double> { juce::Range<double> { tempoSequence.getTempos()[0]->minBPM
                                                                   , tempoSequence.getTempos()[0]->maxBPM }
                                             , tempoSequence.getTempos()[0]->getBpm()
                                             , 1.0
                                             , 10.0 }
    , mTempoSequence { tempoSequence }
{

}

TempoSelectorComponent::~TempoSelectorComponent()
{
}

void TempoSelectorComponent::onValueChange()
{
    mTempoSequence.getTempos()[0]->setBpm(getCurrentValue());
}
