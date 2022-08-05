#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

class MacroMap
{
public:
    MacroMap()
    {
        
    }
    
    ~MacroMap()
    {
        
    }
    
    void setMacro1Group(juce::String parameterToControl)
    {
        macro1MapVector.push_back(parameterToControl);
        saveParam(parameterToControl);
    }
    
    void saveParam(juce::String newParameter)
    {
        juce::String param = "param";
        param.append(std::to_string(macro1MapVector.size() - 1), 4);
        macroParametersTree.setProperty(param, newParameter, nullptr);
    }
    
    juce::String getParameter(int vectorIndex)
    {
        return macro1MapVector[vectorIndex];
    }
    
    void deleteMapping(juce::String parameterMapToDelete)
    {
        for (int i = 0; i < macro1MapVector.size(); i++)
        {
            if (getParameter(i) == parameterMapToDelete)
            {
                macro1MapVector.erase(macro1MapVector.begin() + i);
                macroParametersTree.removeProperty(macroParametersTree.getPropertyName(i), nullptr);
            }
        }
    }
    
    std::vector<juce::String> getMacro1Map()
    {
        return macro1MapVector;
    }
    
    juce::ValueTree macroParametersTree {"MacroParameters"};
    
private:
    
    std::vector<juce::String> macro1MapVector;
    
};
