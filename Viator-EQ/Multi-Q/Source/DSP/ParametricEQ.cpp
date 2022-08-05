#include "ParametricEQ.h"

template <typename SampleType>
ParametricEQ<SampleType>::ParametricEQ() :
mGlobalBypass(false)
{
}

template <typename SampleType>
void ParametricEQ<SampleType>::prepare(const juce::dsp::ProcessSpec& spec)
{
    mCurrentSampleRate = spec.sampleRate;
    
    mGainDB.reset(mCurrentSampleRate, 0.02);
    mGainDB.setTargetValue(0.0);
    
    mRawGain.reset(mCurrentSampleRate, 0.02);
    mRawGain.setTargetValue(0.0);
    
    filter1.prepare(spec);
    filter1.setStereoType(viator_dsp::SVFilter<float>::StereoId::kStereo);
    filter1.setParameter(viator_dsp::SVFilter<float>::ParameterId::kType, viator_dsp::SVFilter<float>::FilterType::kLowShelf);
    filter1.setParameter(viator_dsp::SVFilter<float>::ParameterId::kQType, viator_dsp::SVFilter<float>::QType::kParametric);
    filter1.setParameter(viator_dsp::SVFilter<float>::ParameterId::kCutoff, 200);
    
    filter2.prepare(spec);
    filter2.setStereoType(viator_dsp::SVFilter<float>::StereoId::kStereo);
    filter2.setParameter(viator_dsp::SVFilter<float>::ParameterId::kType, viator_dsp::SVFilter<float>::FilterType::kBandShelf);
    filter2.setParameter(viator_dsp::SVFilter<float>::ParameterId::kQType, viator_dsp::SVFilter<float>::QType::kParametric);
    filter2.setParameter(viator_dsp::SVFilter<float>::ParameterId::kCutoff, 1000);
    
    filter3.prepare(spec);
    filter3.setStereoType(viator_dsp::SVFilter<float>::StereoId::kStereo);
    filter3.setParameter(viator_dsp::SVFilter<float>::ParameterId::kType, viator_dsp::SVFilter<float>::FilterType::kBandShelf);
    filter3.setParameter(viator_dsp::SVFilter<float>::ParameterId::kQType, viator_dsp::SVFilter<float>::QType::kParametric);
    filter3.setParameter(viator_dsp::SVFilter<float>::ParameterId::kCutoff, 1000);
    
    filter4.prepare(spec);
    filter4.setStereoType(viator_dsp::SVFilter<float>::StereoId::kStereo);
    filter4.setParameter(viator_dsp::SVFilter<float>::ParameterId::kType, viator_dsp::SVFilter<float>::FilterType::kHighShelf);
    filter4.setParameter(viator_dsp::SVFilter<float>::ParameterId::kQType, viator_dsp::SVFilter<float>::QType::kParametric);
    filter4.setParameter(viator_dsp::SVFilter<float>::ParameterId::kCutoff, 5000);
}

template <typename SampleType>
void ParametricEQ<SampleType>::setParameter(ParameterId parameter, SampleType parameterValue)
{
    using f = viator_dsp::SVFilter<float>::ParameterId;
    switch (parameter)
    {
        case ParameterId::kFilter1Gain: filter1.setParameter(f::kGain, parameterValue); break;
        case ParameterId::kFilter2Gain: filter2.setParameter(f::kGain, parameterValue); break;
        case ParameterId::kFilter3Gain: filter3.setParameter(f::kGain, parameterValue); break;
        case ParameterId::kFilter4Gain: filter4.setParameter(f::kGain, parameterValue); break;
        case ParameterId::kFilter1Freq: filter1.setParameter(f::kCutoff, parameterValue); break;
        case ParameterId::kFilter2Freq: filter2.setParameter(f::kCutoff, parameterValue); break;
        case ParameterId::kFilter3Freq: filter3.setParameter(f::kCutoff, parameterValue); break;
        case ParameterId::kFilter4Freq: filter4.setParameter(f::kCutoff, parameterValue); break;
        case ParameterId::kSampleRate: mCurrentSampleRate = parameterValue; break;
        case ParameterId::kBypass: mGlobalBypass = static_cast<bool>(parameterValue);
    }
}

template <typename SampleType>
void ParametricEQ<SampleType>::setStereoType(int newStereoID)
{
    switch (newStereoID)
    {
        case 0:
        {
            filter1.setStereoType(viator_dsp::SVFilter<float>::StereoId::kStereo);
            filter2.setStereoType(viator_dsp::SVFilter<float>::StereoId::kStereo);
            filter3.setStereoType(viator_dsp::SVFilter<float>::StereoId::kStereo);
            filter4.setStereoType(viator_dsp::SVFilter<float>::StereoId::kStereo);
            break;
        }
            
        case 1:
        {
            filter1.setStereoType(viator_dsp::SVFilter<float>::StereoId::kMids);
            filter2.setStereoType(viator_dsp::SVFilter<float>::StereoId::kMids);
            filter3.setStereoType(viator_dsp::SVFilter<float>::StereoId::kMids);
            filter4.setStereoType(viator_dsp::SVFilter<float>::StereoId::kMids);
            break;
        }
            
        case 2:
        {
            filter1.setStereoType(viator_dsp::SVFilter<float>::StereoId::kSides);
            filter2.setStereoType(viator_dsp::SVFilter<float>::StereoId::kSides);
            filter3.setStereoType(viator_dsp::SVFilter<float>::StereoId::kSides);
            filter4.setStereoType(viator_dsp::SVFilter<float>::StereoId::kSides);
            break;
        }
    }
    

}

template class ParametricEQ<float>;
template class ParametricEQ<double>;
