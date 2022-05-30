/*
  ==============================================================================

    MixerParametersAreaComponent.cpp
    Created: 3 May 2022 11:33:26am
    Author:  User

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MixerParametersAreaComponent.h"

//==============================================================================
MixerParametersAreaComponent::MixerParametersAreaComponent()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

MixerParametersAreaComponent::~MixerParametersAreaComponent()
{
}

void MixerParametersAreaComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("MixerParametersAreaComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void MixerParametersAreaComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
