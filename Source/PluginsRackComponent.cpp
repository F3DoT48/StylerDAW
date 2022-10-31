/*
  ==============================================================================

    PluginsRackComponent.cpp
    Created: 18 Jul 2022 5:40:15pm
    Author:  User

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PluginsRackComponent.h"

using namespace styler_app;

class PluginTreeBase
{
public:
    virtual ~PluginTreeBase() = default;
    virtual juce::String getUniqueName() const = 0;
    
    void addSubItem (PluginTreeBase* itm)   { mSubitems.add (itm);       }
    int getNumSubItems()                    { return mSubitems.size();   }
    PluginTreeBase* getSubItem (int idx)    { return mSubitems[idx];     }
    
private:
    juce::OwnedArray<PluginTreeBase> mSubitems;
};

//==============================================================================
class PluginTreeItem : public PluginTreeBase
{
public:
    PluginTreeItem (const juce::PluginDescription&);
    PluginTreeItem (const juce::String& uniqueId
                  , const juce::String& name
                  , const juce::String& xmlType
                  , bool isSynth
                  , bool isPlugin);

    te::Plugin::Ptr create (te::Edit&);
    
    juce::String getUniqueName() const override
    {
        //if (mDesc.fileOrIdentifier.startsWith (te::RackType::getRackPresetPrefix()))
        //    return mDesc.fileOrIdentifier;

        return mDesc.createIdentifierString();
    }

    juce::PluginDescription mDesc;
    juce::String mXmlType;
    bool mIsPlugin = true;

    JUCE_LEAK_DETECTOR (PluginTreeItem)
};

class PluginTreeGroup : public PluginTreeBase
{
public:
    PluginTreeGroup (te::Edit&, juce::KnownPluginList::PluginTree&, te::Plugin::Type);
    PluginTreeGroup (const juce::String&);
    
    juce::String getUniqueName() const override           { return mName; }

    juce::String mName;

private:
    void populateFrom (juce::KnownPluginList::PluginTree&);
    //void createBuiltInItems (int& num, te::Plugin::Type);

    JUCE_LEAK_DETECTOR (PluginTreeGroup)
};

//==============================================================================
PluginTreeItem::PluginTreeItem (const juce::PluginDescription& d)
    : mDesc (d),mXmlType (te::ExternalPlugin::xmlTypeName), mIsPlugin (true)
{
    jassert (mXmlType.isNotEmpty());
}

PluginTreeItem::PluginTreeItem (const juce::String& uniqueId, const juce::String& name,
                                const juce::String& xmlType_, bool isSynth, bool isPlugin_)
    : mXmlType (xmlType_), mIsPlugin (isPlugin_)
{
    jassert (mXmlType.isNotEmpty());
    mDesc.name = name;
    mDesc.fileOrIdentifier = uniqueId;
    //mDesc.pluginFormatName = (uniqueId.endsWith ("_trkbuiltin") || mXmlType == te::RackInstance::xmlTypeName)
    //                            ? getInternalPluginFormatName() : juce::String();
    mDesc.pluginFormatName = juce::String();
    mDesc.category = mXmlType;
    mDesc.isInstrument = isSynth;
}

te::Plugin::Ptr PluginTreeItem::create (te::Edit& ed)
{
    return ed.getPluginCache().createNewPlugin (mXmlType, mDesc);
}

//==============================================================================
PluginTreeGroup::PluginTreeGroup (te::Edit& edit, juce::KnownPluginList::PluginTree& tree, te::Plugin::Type types)
    : mName ("Plugins")
{
    populateFrom (tree);
}

PluginTreeGroup::PluginTreeGroup (const juce::String& s)  : mName (s)
{
    jassert (mName.isNotEmpty());
}

void PluginTreeGroup::populateFrom (juce::KnownPluginList::PluginTree& tree)
{
    for (auto subTree : tree.subFolders)
    {
        if (subTree->plugins.size() > 0 || subTree->subFolders.size() > 0)
        {
            auto fs = new PluginTreeGroup (subTree->folder);
            addSubItem (fs);

            fs->populateFrom (*subTree);
        }
    }

    for (const auto& pd : tree.plugins)
        addSubItem (new PluginTreeItem (pd));
}

class PluginMenu : public juce::PopupMenu
{
public:
    PluginMenu() = default;

    PluginMenu (PluginTreeGroup& node)
    {
        for (int i = 0; i < node.getNumSubItems(); ++i)
            if (auto subNode = dynamic_cast<PluginTreeGroup*> (node.getSubItem (i)))
                addSubMenu (subNode->mName, PluginMenu (*subNode), true);

        for (int i = 0; i < node.getNumSubItems(); ++i)
            if (auto subType = dynamic_cast<PluginTreeItem*> (node.getSubItem (i)))
                addItem (subType->getUniqueName().hashCode(), subType->mDesc.name, true, false);
    }

    static PluginTreeItem* findType (PluginTreeGroup& node, int hash)
    {
        for (int i = 0; i < node.getNumSubItems(); ++i)
            if (auto subNode = dynamic_cast<PluginTreeGroup*> (node.getSubItem (i)))
                if (auto* t = findType (*subNode, hash))
                    return t;

        for (int i = 0; i < node.getNumSubItems(); ++i)
            if (auto t = dynamic_cast<PluginTreeItem*> (node.getSubItem (i)))
                if (t->getUniqueName().hashCode() == hash)
                    return t;

        return nullptr;
    }

    PluginTreeItem* runMenu (PluginTreeGroup& node)
    {
        int res = show();

        if (res == 0)
            return nullptr;

        return findType (node, res);
    }
};

inline std::unique_ptr<juce::KnownPluginList::PluginTree> createPluginTree (te::Engine& engine)
{
    auto& list = engine.getPluginManager().knownPluginList;

    if (auto tree = list.createTree (list.getTypes(), juce::KnownPluginList::sortByManufacturer))
        return tree;

    return {};
}

te::Plugin::Ptr showMenuAndCreatePlugin (te::Edit& edit)
{
    if (auto tree = createPluginTree (edit.engine))
    {
        PluginTreeGroup root (edit, *tree, te::Plugin::Type::allPlugins);
        PluginMenu m (root);

        if (auto type = m.runMenu (root))
            return type->create (edit);
    }
    
    return {};
}

//==============================================================================
PluginsRackComponent::PluginsRackComponent (te::RackInstance* rackInstance)
    : mRackInstance { rackInstance }
    , mAddPluginButton { "Add Plugin" }
    , mUpdatePlugins { false }
    , mUpdateWires { false }
    , mMidiPinToPlugins { std::make_unique<PinComponent> (te::EditItemID{}, 0, false, juce::String {"MIDI"}) }
    , mAudioLeftPinToPlugins { std::make_unique<PinComponent> (te::EditItemID{}, 1, false, juce::String { "Left" }) }
    , mAudioRightPinToPlugins { std::make_unique<PinComponent> (te::EditItemID{}, 2, false, juce::String { "Right" }) }
    , mAudioLeftPinFromPlugins { std::make_unique<PinComponent> (te::EditItemID{}, 1, true, juce::String { "Left" }) }
    , mAudioRightPinFromPlugins { std::make_unique<PinComponent> (te::EditItemID{}, 2, true, juce::String { "Right" } )}
    , mDrawableWire { std::make_unique<WireComponent> (juce::Point<int> { 0, 0 }, juce::Point<int> { 0, 0 }) }
{
    mAddPluginButton.onClick = [this]()
    {
        if (auto plugin = showMenuAndCreatePlugin (mRackInstance->getOwnerTrack()->edit))
        {
            mRackInstance->type->addPlugin (plugin, juce::Point<float>(0.1f, 0.1f), false);
        }
    };

    addAndMakeVisible (mAddPluginButton);

    mRackInstance->type->state.addListener (this);

    addAndMakeVisible (mMidiPinToPlugins.get());
    addAndMakeVisible (mAudioLeftPinToPlugins.get());
    addAndMakeVisible (mAudioRightPinToPlugins.get());
    addAndMakeVisible (mAudioLeftPinFromPlugins.get());
    addAndMakeVisible (mAudioRightPinFromPlugins.get());

    addChildComponent (mDrawableWire.get());
        
    markAndUpdate (mUpdatePlugins);
    markAndUpdate (mUpdateWires);
}

PluginsRackComponent::~PluginsRackComponent()
{
    mRackInstance->type->state.removeListener (this);
}

void PluginsRackComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    //g.setColour (juce::Colours::white);
    //g.setFont (14.0f);
    //g.drawText ("PluginsRackComponent", getLocalBounds(),
    //            juce::Justification::centred, true);   // draw some placeholder text
}

void PluginsRackComponent::resized()
{
    auto rect { getLocalBounds() };

    auto rectForButtons { rect.removeFromTop (30) };

    mAddPluginButton.setBounds (rectForButtons.removeFromRight (mAddPluginButton.getLookAndFeel().getTextButtonWidthToFitText (mAddPluginButton
                                                                                                                            , rectForButtons.getHeight())));

    mMidiPinToPlugins->setBounds (5
                                , (getHeight() - PinComponent::sHeightInPixels) / 2 - 4 - PinComponent::sHeightInPixels
                                , PinComponent::sWidthInPixels
                                , PinComponent::sHeightInPixels);

    mAudioLeftPinToPlugins->setBounds (5
                                    , (getHeight() - PinComponent::sHeightInPixels) / 2
                                    , PinComponent::sWidthInPixels
                                    , PinComponent::sHeightInPixels);
    mAudioRightPinToPlugins->setBounds (5
                                     , (getHeight() + PinComponent::sHeightInPixels) / 2 + 4
                                     , PinComponent::sWidthInPixels
                                     , PinComponent::sHeightInPixels);

    mAudioLeftPinFromPlugins->setBounds (getWidth() - PinComponent::sWidthInPixels - 5
                                       , getHeight() / 2 - 2 - PinComponent::sHeightInPixels
                                       , PinComponent::sWidthInPixels
                                       , PinComponent::sHeightInPixels);
    mAudioRightPinFromPlugins->setBounds (getWidth() - PinComponent::sWidthInPixels - 5
                                        , getHeight() / 2 + 2 + PinComponent::sHeightInPixels
                                        , PinComponent::sWidthInPixels
                                        , PinComponent::sHeightInPixels);

    for (auto plugin : mPlugins)
    {
        juce::StringArray ins, outs;
        plugin->getPlugin()->getChannelNames (&ins, &outs);
        const int maxNumPins { juce::jmax (ins.size()
                                         , outs.size()) };

        const int pluginHeight { PluginInRackComponent::sMinHeightInPixels
                               + (maxNumPins + 1) * PinComponent::sHeightInPixels
                               + (maxNumPins + 1) * PluginInRackComponent::sGapBtwPins };

        auto position { mRackInstance->type->getPluginPosition(plugin->getPlugin())};
        plugin->setBounds (static_cast<int> (position.getX() * PluginsRackComponent::sViewportWidth)
                         , static_cast<int> (position.getY() * PluginsRackComponent::sViewportHeight)
                         , PluginInRackComponent::sWidthInPixels
                         , pluginHeight);
    }

    rebuildWiresUsingPlugins();

    for (auto wire : mWires)
    {
        wire->setBounds (juce::jmin (wire->getBeginInRack().getX()
                                   , wire->getEndInRack().getX())
                       , juce::jmin (wire->getBeginInRack().getY()
                                   , wire->getEndInRack().getY())
                       , std::abs (wire->getBeginInRack().getX() - wire->getEndInRack().getX())
                       , std::abs (wire->getBeginInRack().getY() - wire->getEndInRack().getY()));
    }

    mDrawableWire->setBounds (juce::jmin (mDrawableWire->getBeginInRack().getX()
                                        , mDrawableWire->getEndInRack().getX())
                                        , juce::jmin (mDrawableWire->getBeginInRack().getY()
                                        , mDrawableWire->getEndInRack().getY())
                                        , std::abs (mDrawableWire->getBeginInRack().getX() - mDrawableWire->getEndInRack().getX())
                                        , std::abs (mDrawableWire->getBeginInRack().getY() - mDrawableWire->getEndInRack().getY()));
}

void PluginsRackComponent::valueTreeChanged()
{

}

void PluginsRackComponent::valueTreeChildAdded (juce::ValueTree&, juce::ValueTree& child)
{
    if (child.hasType (te::IDs::PLUGININSTANCE))
    {
        markAndUpdate (mUpdatePlugins);
    }

    if (child.hasType (te::IDs::CONNECTION))
    {
        markAndUpdate (mUpdateWires);
    }
}

void PluginsRackComponent::valueTreeChildRemoved (juce::ValueTree&, juce::ValueTree& child, int)
{
    if (child.hasType (te::IDs::PLUGININSTANCE))
    {
        markAndUpdate (mUpdatePlugins);
    }

    if (child.hasType (te::IDs::CONNECTION))
    {
        markAndUpdate (mUpdateWires);
    }
}

void PluginsRackComponent::valueTreeChildOrderChanged (juce::ValueTree&, int, int)
{
    markAndUpdate (mUpdatePlugins);
    markAndUpdate (mUpdateWires);
}

void PluginsRackComponent::handleAsyncUpdate()
{
    if (compareAndReset (mUpdateWires))
    {
        buildWires();
    }

    if (compareAndReset (mUpdatePlugins))
    {
        buildPlugins();
    }
}

void PluginsRackComponent::buildPlugins()
{
    mPlugins.clear();
    
    te::RackType::Ptr rackType { mRackInstance->type };
 
    for (const auto plugin : rackType->getPlugins())
    {
        auto p = new PluginInRackComponent (plugin);
        addAndMakeVisible (p);
        mPlugins.add (p);
    }

    rebuildWiresUsingPlugins();

    resized();
}

void PluginsRackComponent::buildWires()
{
    rebuildWiresUsingPlugins();
    resized();
}

void PluginsRackComponent::rebuildWiresUsingPlugins()
{
    mWires.clear();

    te::RackType::Ptr rackType { mRackInstance->type };

    for (const auto connection : rackType->getConnections())
    {
        juce::Point<int> wireBegin;
        if (connection->sourceID.get().toString().compare ("0") == 0 && connection->sourcePin == 0)
        {
            wireBegin = mMidiPinToPlugins->getWireSnapPositionInRack();
        }
        else if (connection->sourceID.get().toString().compare ("0") == 0 && connection->sourcePin == 1)
        {
            wireBegin = mAudioLeftPinToPlugins->getWireSnapPositionInRack();
        }
        else if (connection->sourceID.get().toString().compare ("0") == 0 && connection->sourcePin == 2)
        {
            wireBegin = mAudioRightPinToPlugins->getWireSnapPositionInRack();
        }

        juce::Point<int> wireEnd;
        if (connection->destID.get().toString().compare ("0") == 0 && connection->destPin == 0)
        {
            continue; // skip the midi from plugins pin
        }
        else if (connection->destID.get().toString().compare ("0") == 0 && connection->destPin == 1)
        {
            wireEnd = mAudioLeftPinFromPlugins->getWireSnapPositionInRack();
        }
        else if (connection->destID.get().toString().compare ("0") == 0 && connection->destPin == 2)
        {
            wireEnd = mAudioRightPinFromPlugins->getWireSnapPositionInRack();
        }

        for (PluginInRackComponent* const plugin : mPlugins)
        {
            if (plugin->getPlugin()->itemID == connection->sourceID)
            {
                wireBegin = plugin->getOutputPins()[connection->sourcePin]->getWireSnapPositionInRack();
            }
            if (plugin->getPlugin()->itemID == connection->destID)
            {
                wireEnd = plugin->getInputPins()[connection->destPin]->getWireSnapPositionInRack();
            }
        }

        auto wire = new WireComponent (wireBegin
                                     , wireEnd);
        addAndMakeVisible (wire, 0);

        mWires.add (wire);
    }
}

WireComponent& PluginsRackComponent::getDrawableWire() const
{
    return *mDrawableWire;
}


te::RackInstance* PluginsRackComponent::getRackInstance() const
{
    return mRackInstance;
}