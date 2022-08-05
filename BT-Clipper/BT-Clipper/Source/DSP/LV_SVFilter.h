/*
  ==============================================================================

    LV_EQ.h
    Created: 25 May 2021 5:40:30pm
    Author:  Landon Viator

  ==============================================================================
*/

#ifndef LV_SVFilter_h
#define LV_SVFilter_h

#include <JuceHeader.h>

class LV_SVFilter
{
public:
    
    void reset();
    
    void prepare(juce::dsp::ProcessSpec spec);
    
    void processBlock(juce::dsp::AudioBlock<float>& block);
    
    enum class ParameterId
    {
        kType,
        kCutoff,
        kQ,
        kGain,
        kQType,
        kSampleRate,
        kBypass,
        kClipOutput
    };
        
    enum FilterType
    {
        kLowShelf,
        kBandShelf,
        kHighShelf,
        kLowPass,
        kHighPass,
    };
    
    enum QType
    {
        kParametric,
        kProportional,
    };
    
    void setParameter(ParameterId parameter, float parameterValue);
    void setGain(float value);
    float getShelfQ(float value) const;
    float getPeakQ(float value) const;
    
private:
    
    // Member variables
    float mCurrentSampleRate {44100.0f};
    float mQ {0.1f};
    float mCutoff {1000.0f};
    float mGain {1.0f};
    float mRawGain {0.0f};
    bool mGlobalBypass {false};
    bool mClipOutput {false};
    
    // Filter type switch
    FilterType mType {FilterType::kLowPass};
    
    // Q mode switch
    QType mQType {QType::kParametric};
    
    // Variables for the Z filter equations
    double mGCoeff {0.0}; // gain element
    double mRCoeff {0.0}; // feedback damping element
    double mRCoeff2 {0.0};
    double mK {1.0};
    double mInversion {0.0};
    
    float twoPi = juce::MathConstants<float>::twoPi;
        
    std::vector<double> mZ1, mZ2; // state variables (z^-1)
};

#endif /* LV_SVFilter_h */
