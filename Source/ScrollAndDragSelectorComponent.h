/*
  ==============================================================================

    ScrollAndDragSelectorComponent.h
    Created: 19 Aug 2023 5:54:09pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

namespace styler_app
{
    template<typename NumericType>
    class ScrollAndDragSelectorComponent  : public juce::Component
    {
    public:
        ScrollAndDragSelectorComponent (const juce::Range<NumericType>& range
                                      , NumericType startValue
                                      , NumericType step
                                      , NumericType stepModifier)
            : mRange { range }
            , mStep { step }
            , mModifier { stepModifier }
            , mCurrentValue { startValue }
            , mValueOnDragBegin { startValue }
            , mValueLabel { "valueLabel", juce::String{} }
        {
            if (mCurrentValue < mRange.getStart()
             || mCurrentValue > mRange.getEnd())
            {
                mCurrentValue = mRange.getStart();
            }

            if constexpr (std::is_floating_point_v<NumericType>)
            {
                mValueLabel.setText (juce::String (mCurrentValue, 0)
                                   , juce::dontSendNotification);
            }
            else
            {
                mValueLabel.setText (juce::String (mCurrentValue)
                                   , juce::dontSendNotification);
            }

            mValueLabel.setEditable (false);
            mValueLabel.setJustificationType (juce::Justification::right);
            mValueLabel.setInterceptsMouseClicks (false, false);
            addAndMakeVisible (mValueLabel);
        }

        void paint (juce::Graphics& g) override
        {
            g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

            g.setColour (juce::Colours::grey);
            g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
        }

        void resized() override
        {
            mValueLabel.setBounds (getLocalBounds());
        }

        void mouseWheelMove (const juce::MouseEvent& event
                           , const juce::MouseWheelDetails& wheel) override
        {
            const auto step { event.mods.getCurrentModifiers().isCtrlDown() 
                            ? mModifier : static_cast<NumericType> (1) 
                            * mStep };

            const auto delta { wheel.deltaY };

            const auto newValue { mCurrentValue 
                                + static_cast<int> (std::copysign (static_cast<decltype(delta)> (1)
                                                                 , delta))
                                * step };

            changeValue (newValue);
        }

        void mouseDown (const juce::MouseEvent&) override
        {
            mValueOnDragBegin = mCurrentValue;
        }

        void mouseDrag (const juce::MouseEvent& event) override
        {
            const auto step { event.mods.getCurrentModifiers().isCtrlDown() 
                            ? mModifier : static_cast<NumericType> (1)
                            * mStep };
            
            const auto delta { static_cast<NumericType> (event.getDistanceFromDragStartY()) };

            const auto newValue { mValueOnDragBegin
                                - static_cast<int> (delta / step)
                                * step };

            changeValue (newValue);
        }

        const juce::Range<NumericType>& getRange() const
        {
            return mRange;
        }

        NumericType getStep() const
        {
            return mStep;
        }

        NumericType getCurrentValue() const
        {
            return mCurrentValue;
        }

    private:
        const juce::Range<NumericType> mRange;
        const NumericType mStep;
        const NumericType mModifier;
        NumericType mCurrentValue;
        NumericType mValueOnDragBegin;

        juce::Label mValueLabel;

        void changeValue (const NumericType newValue)
        {
            if constexpr (std::is_floating_point_v<NumericType>)
            {
                const NumericType tol {static_cast<NumericType> (0.1f) * mStep};

                if (newValue > mRange.getStart() - tol
                 && newValue < mRange.getEnd() + tol)
                {
                    mCurrentValue = newValue;
                    mValueLabel.setText (juce::String (mCurrentValue, 0)
                                       , juce::dontSendNotification);
                }
            }
            else
            {
                if (newValue >= mRange.getStart()
                 && newValue <= mRange.getEnd())
                {
                    mCurrentValue = newValue;
                    mValueLabel.setText (juce::String (mCurrentValue)
                                       , juce::dontSendNotification);
                }            
            }

            onValueChange();
        }

        virtual void onValueChange() = 0;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ScrollAndDragSelectorComponent)
    };
}