/*
  ==============================================================================

    PinComponent.cpp
    Created: 16 Oct 2022 11:32:55am
    Author:  User

  ==============================================================================
*/

#include "PinComponent.h"
#include "PluginsRackComponent.h"
#include "WireComponent.h"

using namespace styler_app;

PinComponent::PinComponent(te::EditItemID editItemId
                         , const int pinIndex
                         , const bool isInputPin
                         , const juce::String& name)
    : ConnectionTarget (editItemId, pinIndex, isInputPin)
    , mName {name}
{

}

PinComponent::~PinComponent()
{

}

void PinComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (isInputPin() ? juce::Colours::green : juce::Colours::red);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (11.0f);
    juce::String text {mPinIndex};
    text += ":";
    text += mName;
    g.drawText (text, getLocalBounds(), juce::Justification::left, true);   // draw some placeholder text
}

void PinComponent::mouseDown(const juce::MouseEvent& mouseEvent)
{
    if (! mouseEvent.mods.isPopupMenu())
    {
        auto pluginsRackParent { findParentComponentOfClass<PluginsRackComponent>() };
        auto& drawableWire = pluginsRackParent->getDrawableWire();

        drawableWire.setBeginInRack (getWireSnapPositionInRack());
        drawableWire.setEndInRack (pluginsRackParent->getLocalPoint (this, mouseEvent.getPosition()));
        pluginsRackParent->resized();
        drawableWire.setVisible (true);
    }
    else if (mouseEvent.mods.isPopupMenu() && isInputPin())
    {
        
        juce::PopupMenu wiresMenu;
        wiresMenu.addItem (-1, "Remove:", false, false);
       
        int itemId { 1 };
        auto pluginsRackParent { findParentComponentOfClass<PluginsRackComponent>() };
        juce::Array<const te::RackConnection*> wiresToThisPin;
            
        for (const auto& connection : pluginsRackParent->getRackInstance()->type->getConnections())
        {
            if (connection->destID == getEditItemID() && connection->destPin == getPinIndex())
            {
                juce::String itemName {};
                if (connection->sourceID == te::EditItemID {})
                {
                    itemName += "From arranger: ";
                    switch (connection->sourcePin)
                    {
                        case 0:
                            itemName += "MIDI";
                            break;
                        case 1:
                            itemName += "Left";
                            break;
                        case 2:
                            itemName += "Right";
                            break;
                        default:
                            jassertfalse;
                    }
                }
                else
                {
                    itemName += pluginsRackParent->getRackInstance()->type->getPluginForID (connection->sourceID)->getName();
                    itemName += ": ";
                    if (connection->sourcePin == 0)
                    {
                        itemName += "MIDI";
                    }
                    else
                    {
                        juce::StringArray ins, outs;
                        pluginsRackParent->getRackInstance()->type->getPluginForID (connection->sourceID)->getChannelNames (&ins, &outs);
                        itemName += outs[connection->sourcePin - 1];
                    }
                }
                wiresMenu.addItem (itemId++
                                 , itemName
                                 , true
                                 , false);
                wiresToThisPin.add (connection);
            }
        }

        int clickedItemId { wiresMenu.show() };

        if (clickedItemId > 0)
        {
            const auto connectionToRemove { wiresToThisPin[clickedItemId - 1] };
            pluginsRackParent->getRackInstance()->type->removeConnection (connectionToRemove->sourceID
                                                                        , connectionToRemove->sourcePin
                                                                        , connectionToRemove->destID
                                                                        , connectionToRemove->destPin);
        }
    }
}

void PinComponent::mouseUp (const juce::MouseEvent& mouseEvent)
{
    if (! mouseEvent.mods.isPopupMenu())
    {
        auto pluginsRackParent { findParentComponentOfClass<PluginsRackComponent>() };

        if (const PinComponent* dstPin { dynamic_cast<PinComponent*> (pluginsRackParent->getComponentAt (pluginsRackParent->getLocalPoint (this, mouseEvent.getPosition()))) };
            dstPin != nullptr && dstPin != this)
        {
            pluginsRackParent->getRackInstance()->type->addConnection (getEditItemID()
                                                                     , getPinIndex()
                                                                     , dstPin->getEditItemID()
                                                                     , dstPin->getPinIndex());
        }

        pluginsRackParent->getDrawableWire().setVisible (false);
        pluginsRackParent->resized();
    }
}

void PinComponent::mouseDrag(const juce::MouseEvent& mouseEvent)
{
    if (! mouseEvent.mods.isPopupMenu())
    {
        auto pluginsRackParent { findParentComponentOfClass<PluginsRackComponent>() };
        auto& drawableWire = pluginsRackParent->getDrawableWire();

        drawableWire.setEndInRack (pluginsRackParent->getLocalPoint (this, mouseEvent.getPosition()));
        pluginsRackParent->resized();
    }
       
    beginDragAutoRepeat (100);
}

juce::Point<int> PinComponent::getWireSnapPositionInRack() const noexcept
{
    const auto localPoint { juce::Point<int> { isInputPin() ? 0 : sWidthInPixels, sHeightInPixels / 2 }};
    const auto parentRackComponent { findParentComponentOfClass<PluginsRackComponent>() };
    return parentRackComponent->getLocalPoint (this, localPoint);
}

