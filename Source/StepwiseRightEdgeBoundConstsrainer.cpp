/*
  ==============================================================================

    StepwiseRightEdgeBoundConstsrainer.cpp
    Created: 17 Sep 2023 12:50:10pm
    Author:  User

  ==============================================================================
*/

#include "StepwiseRightEdgeBoundConstsrainer.h"

using namespace styler_app;

StepwiseRightEdgeBoundConstsrainer::StepwiseRightEdgeBoundConstsrainer (int /* numPixelsPerBeat */
                                                                      , const PRToolBarComponent& toolBar)
    : juce::ComponentBoundsConstrainer {}
    , mNumPixelsPerBeat { 1 }
    , mToolBar { toolBar }
{

}

void StepwiseRightEdgeBoundConstsrainer::updateNumPixelsPerBeat (int newNumPixelsPerBeat)
{
    mNumPixelsPerBeat = newNumPixelsPerBeat;
    setMinimumWidth (static_cast<int> (mToolBar.getSnapResolutionInBeats()
                                     * mNumPixelsPerBeat));
}

void StepwiseRightEdgeBoundConstsrainer::checkBounds (juce::Rectangle<int>& bounds
                                                    , const juce::Rectangle<int>& previousBounds
                                                    , const juce::Rectangle<int>& limits
                                                    , bool isStretchingTop
                                                    , bool isStretchingLeft
                                                    , bool isStretchingBottom
                                                    , bool isStretchingRight)
{
    juce::ComponentBoundsConstrainer::checkBounds (bounds
                                                 , previousBounds
                                                 , limits
                                                 , isStretchingTop
                                                 , isStretchingLeft
                                                 , isStretchingBottom
                                                 , isStretchingRight);

    const int extraPixels { bounds.getRight() - previousBounds.getRight() };

    const int correctionIfShorteningTheNote { extraPixels < 0 ? 1 : 0 };

    const int snapMultiple { static_cast<int> (extraPixels
                                             / (mNumPixelsPerBeat * mToolBar.getSnapResolutionInBeats()))
                           - correctionIfShorteningTheNote };

    bounds.setRight (static_cast<int> (previousBounds.getRight()
                                     + snapMultiple
                                     * mToolBar.getSnapResolutionInBeats()
                                     * mNumPixelsPerBeat));

    juce::ComponentBoundsConstrainer::checkBounds (bounds
                                                 , previousBounds
                                                 , limits
                                                 , isStretchingTop
                                                 , isStretchingLeft
                                                 , isStretchingBottom
                                                 , isStretchingRight);
}