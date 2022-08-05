#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

class Vector3D
{
public:
    int channel;
    int cc;
    juce::String parameter;
};

class MidiMap
{
    
public:
    
    MidiMap()
    {
    }
    
    void setMidiGroup(int newChannel, int newCC, juce::String newParameter)
    {
        midiVector.push_back({newChannel, newCC, newParameter});
        saveChannel(newChannel);
        saveCC(newCC);
        saveParam(newParameter);
    }
    
    void saveChannel(int newChannel)
    {
        juce::String channel = "channel";
        channel.append(std::to_string(midiVector.size() - 1), 4);
        midiChannelsTree.setProperty(channel, newChannel, nullptr);
    }
    
    void saveCC(int newCC)
    {
        juce::String cc = "cc";
        cc.append(std::to_string(midiVector.size() - 1), 4);
        midiCCTree.setProperty(cc, newCC, nullptr);
    }
    
    void saveParam(juce::String newParameter)
    {
        juce::String param = "param";
        param.append(std::to_string(midiVector.size() - 1), 4);
        midiParametersTree.setProperty(param, newParameter, nullptr);
    }
    
    int getChannelNumber(int vectorIndex)
    {
        return midiVector[vectorIndex].channel;
    }
    
    int getCCNumber(int vectorIndex)
    {
        return midiVector[vectorIndex].cc;
    }
    
    juce::String getParameter(int vectorIndex)
    {
        return midiVector[vectorIndex].parameter;
    }
    
    void deleteMapping(juce::String parameterMapToDelete)
    {
        for (int i = 0; i < midiVector.size(); i++)
        {
            if (getParameter(i) == parameterMapToDelete)
            {
                midiVector.erase(midiVector.begin() + i);
                midiChannelsTree.removeProperty(midiChannelsTree.getPropertyName(i), nullptr);
                midiCCTree.removeProperty(midiCCTree.getPropertyName(i), nullptr);
                midiParametersTree.removeProperty(midiParametersTree.getPropertyName(i), nullptr);
            }
        }
    }
    
    void setCurrentValues(int newChannel, int newCC)
    {
        currentChannel = newChannel;
        currentCC = newCC;
    }
    
    int getcurrentChannel()
    {
        return currentChannel;
    }
    
    int getCurrentCC()
    {
        return currentCC;
    }
    
    void setIsListening(bool newIsListening)
    {
        isListening = newIsListening;
    }
    
    bool getIsListening()
    {
        return isListening;
    }
    
    std::vector<Vector3D> midiVector;
    
    juce::ValueTree midiChannelsTree {"MidiChannels"};
    juce::ValueTree midiCCTree {"MidiCCs"};
    juce::ValueTree midiParametersTree {"MidiParameters"};
    
private:
    
    int currentChannel;
    int currentCC;
    bool isListening = false;
    
};
