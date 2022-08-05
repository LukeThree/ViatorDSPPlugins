
#include "TubeEQ.h"

template <typename SampleType>
TubeEQ<SampleType>::TubeEQ() :
mGlobalBypass(false)
{
}

template <typename SampleType>
void TubeEQ<SampleType>::prepare(const juce::dsp::ProcessSpec& spec)
{
    mCurrentSampleRate = spec.sampleRate;
    
    mGainDB.reset(mCurrentSampleRate, 0.02);
    mGainDB.setTargetValue(0.0);
    
    mRawGain.reset(mCurrentSampleRate, 0.02);
    mRawGain.setTargetValue(1.0);
    
    lowBoostFilter.prepare(spec);
    lowBoostFilter.setStereoType(viator_dsp::SVFilter<float>::StereoId::kStereo);
    lowBoostFilter.setParameter(viator_dsp::SVFilter<float>::ParameterId::kType, viator_dsp::SVFilter<float>::FilterType::kLowShelf);
    lowBoostFilter.setParameter(viator_dsp::SVFilter<float>::ParameterId::kCutoff, 100);
    lowBoostFilter.setParameter(viator_dsp::SVFilter<float>::ParameterId::kQType, viator_dsp::SVFilter<float>::QType::kParametric);
    lowBoostFilter.setParameter(viator_dsp::SVFilter<float>::ParameterId::kQ, 0.1f);
    
    lowCutFilter.prepare(spec);
    lowCutFilter.setStereoType(viator_dsp::SVFilter<float>::StereoId::kStereo);
    lowCutFilter.setParameter(viator_dsp::SVFilter<float>::ParameterId::kType, viator_dsp::SVFilter<float>::FilterType::kBandShelf);
    lowCutFilter.setParameter(viator_dsp::SVFilter<float>::ParameterId::kCutoff, 200);
    lowCutFilter.setParameter(viator_dsp::SVFilter<float>::ParameterId::kQType, viator_dsp::SVFilter<float>::QType::kParametric);
    lowCutFilter.setParameter(viator_dsp::SVFilter<float>::ParameterId::kQ, 0.1f);
    
    highBoostFilter.prepare(spec);
    highBoostFilter.setStereoType(viator_dsp::SVFilter<float>::StereoId::kStereo);
    highBoostFilter.setParameter(viator_dsp::SVFilter<float>::ParameterId::kType, viator_dsp::SVFilter<float>::FilterType::kHighShelf);
    highBoostFilter.setParameter(viator_dsp::SVFilter<float>::ParameterId::kCutoff, 5000);
    highBoostFilter.setParameter(viator_dsp::SVFilter<float>::ParameterId::kQType, viator_dsp::SVFilter<float>::QType::kParametric);
    highBoostFilter.setParameter(viator_dsp::SVFilter<float>::ParameterId::kQ, 0.1f);
    
    highCutFilter.prepare(spec);
    highCutFilter.setStereoType(viator_dsp::SVFilter<float>::StereoId::kStereo);
    highCutFilter.setParameter(viator_dsp::SVFilter<float>::ParameterId::kType, viator_dsp::SVFilter<float>::FilterType::kBandShelf);
    highCutFilter.setParameter(viator_dsp::SVFilter<float>::ParameterId::kCutoff, 2500);
    highCutFilter.setParameter(viator_dsp::SVFilter<float>::ParameterId::kQType, viator_dsp::SVFilter<float>::QType::kParametric);
    highCutFilter.setParameter(viator_dsp::SVFilter<float>::ParameterId::kQ, 0.1f);
}

template <typename SampleType>
void TubeEQ<SampleType>::setParameter(ParameterId parameter, SampleType parameterValue)
{
    using f = viator_dsp::SVFilter<float>::ParameterId;
    
    switch (parameter)
    {
        case ParameterId::kLowBoostGain: lowBoostFilter.setParameter(f::kGain, parameterValue); break;
        case ParameterId::kLowCutGain: lowCutFilter.setParameter(f::kGain, parameterValue); break;
        case ParameterId::kHighBoostGain: highBoostFilter.setParameter(f::kGain, parameterValue); break;
        case ParameterId::kHighCutGain: highCutFilter.setParameter(f::kGain, parameterValue); break;
            
        case ParameterId::kLowFilterFreq:
        {
            lowBoostFilter.setParameter(f::kCutoff, parameterValue);
            lowCutFilter.setParameter(f::kCutoff, parameterValue * 1.5);
            break;
        }
            
        case ParameterId::kHighFilterFreq:
        {
            highBoostFilter.setParameter(f::kCutoff, parameterValue);
            highCutFilter.setParameter(f::kCutoff, parameterValue * 0.75);
            break;
        }
            
        case ParameterId::kBandWidth:
        {
            auto newQ = juce::jmap(static_cast<float>(parameterValue), 0.0f, 10.0f, 0.1f, 0.7f);
            
            lowBoostFilter.setParameter(f::kQ, newQ);
            lowCutFilter.setParameter(f::kQ, newQ);
            highBoostFilter.setParameter(f::kQ, newQ);
            highCutFilter.setParameter(f::kQ, newQ);
            break;
        }
            
        case ParameterId::kDrive:
        {
            mGainDB.setTargetValue(parameterValue);
            mRawGain.setTargetValue(viator_utils::utils::dbToGain(parameterValue));
            break;
        }
            
        case ParameterId::kSampleRate: mCurrentSampleRate = parameterValue; break;
        case ParameterId::kBypass: mGlobalBypass = static_cast<bool>(parameterValue);
    }
}

template <typename SampleType>
void TubeEQ<SampleType>::setStereoType(int newStereoID)
{
    switch (newStereoID)
    {
        case 0:
        {
            lowBoostFilter.setStereoType(viator_dsp::SVFilter<float>::StereoId::kStereo);
            lowCutFilter.setStereoType(viator_dsp::SVFilter<float>::StereoId::kStereo);
            highBoostFilter.setStereoType(viator_dsp::SVFilter<float>::StereoId::kStereo);
            highCutFilter.setStereoType(viator_dsp::SVFilter<float>::StereoId::kStereo);
            break;
        }
            
        case 1:
        {
            lowBoostFilter.setStereoType(viator_dsp::SVFilter<float>::StereoId::kMids);
            lowCutFilter.setStereoType(viator_dsp::SVFilter<float>::StereoId::kMids);
            highBoostFilter.setStereoType(viator_dsp::SVFilter<float>::StereoId::kStereo);
            highCutFilter.setStereoType(viator_dsp::SVFilter<float>::StereoId::kStereo);
            break;
        }
            
        case 2:
        {
            lowBoostFilter.setStereoType(viator_dsp::SVFilter<float>::StereoId::kSides);
            lowCutFilter.setStereoType(viator_dsp::SVFilter<float>::StereoId::kSides);
            highBoostFilter.setStereoType(viator_dsp::SVFilter<float>::StereoId::kStereo);
            highCutFilter.setStereoType(viator_dsp::SVFilter<float>::StereoId::kStereo);
            break;
        }
    }
    

}

template class TubeEQ<float>;
template class TubeEQ<double>;
