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
{
    mAddPluginButton.onClick = [this]()
    {
        if (auto plugin = showMenuAndCreatePlugin (mRackInstance->getOwnerTrack()->edit))
        {
            mRackInstance->type->addPlugin (plugin, juce::Point<float>(100, 100), true);
        }
    };

    addAndMakeVisible (mAddPluginButton);

    mRackInstance->type->state.addListener (this);

    markAndUpdate (mUpdatePlugins);
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

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("PluginsRackComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void PluginsRackComponent::resized()
{
    auto rect { getLocalBounds() };

    auto rectForButtons { rect.removeFromTop (30) };

    mAddPluginButton.setBounds (rectForButtons.removeFromLeft (mAddPluginButton.getLookAndFeel().getTextButtonWidthToFitText (mAddPluginButton
                                                                                                                            , rectForButtons.getHeight())));

    auto rectForPlugins {rect.removeFromTop (100)};
    const int pluginComponentWidth { 100 };
    const int gap { 50 };

    for (auto plugin : mPlugins)
    {
        rectForPlugins.removeFromLeft (gap);
        plugin->setBounds (rectForPlugins.removeFromLeft (pluginComponentWidth));
    }
}

void PluginsRackComponent::valueTreeChanged()
{

}

void PluginsRackComponent::valueTreeChildAdded (juce::ValueTree&, juce::ValueTree& child)
{
    if (child.hasType (te::IDs::PLUGININSTANCE))
        markAndUpdate (mUpdatePlugins);
}

void PluginsRackComponent::valueTreeChildRemoved (juce::ValueTree&, juce::ValueTree& child, int)
{
    if (child.hasType (te::IDs::PLUGININSTANCE))
        markAndUpdate (mUpdatePlugins);
}

void PluginsRackComponent::valueTreeChildOrderChanged (juce::ValueTree&, int, int)
{
    markAndUpdate (mUpdatePlugins);
}

void PluginsRackComponent::handleAsyncUpdate()
{
    if (compareAndReset (mUpdatePlugins))
        buildPlugins();
}

void PluginsRackComponent::buildPlugins()
{
    mPlugins.clear();
    
    te::RackType::Ptr rackType { mRackInstance->type };
    
   // if (rackType != nullptr)
   // {
        for (auto plugin : rackType->getPlugins())
        {
            auto p = new PluginInRackComponent (plugin);
            addAndMakeVisible (p);
            mPlugins.add (p);
        }
   // }
    resized();
}
