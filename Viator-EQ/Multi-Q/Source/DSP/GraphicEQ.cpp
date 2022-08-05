#include "GraphicEQ.h"

template <typename SampleType>
GraphicEQ<SampleType>::GraphicEQ() :
mGlobalBypass(false)
{
}

template <typename SampleType>
void GraphicEQ<SampleType>::prepare(const juce::dsp::ProcessSpec& spec)
{
    mCurrentSampleRate = spec.sampleRate;
    
    mGainDB.reset(mCurrentSampleRate, 0.02);
    mGainDB.setTargetValue(0.0);
    
    mRawGain.reset(mCurrentSampleRate, 0.02);
    mRawGain.setTargetValue(0.0);
    
    filter1.prepare(spec);
    filter1.setStereoType(viator_dsp::SVFilter<float>::StereoId::kStereo);
    filter1.setParameter(viator_dsp::SVFilter<float>::ParameterId::kType, viator_dsp::SVFilter<float>::FilterType::kBandShelf);
    filter1.setParameter(viator_dsp::SVFilter<float>::ParameterId::kSampleRate, mCurrentSampleRate);
    filter1.setParameter(viator_dsp::SVFilter<float>::ParameterId::kCutoff, 31);
    filter1.setParameter(viator_dsp::SVFilter<float>::ParameterId::kQType, viator_dsp::SVFilter<float>::QType::kProportional);
    
    filter2.prepare(spec);
    filter2.setStereoType(viator_dsp::SVFilter<float>::StereoId::kStereo);
    filter2.setParameter(viator_dsp::SVFilter<float>::ParameterId::kType, viator_dsp::SVFilter<float>::FilterType::kBandShelf);
    filter2.setParameter(viator_dsp::SVFilter<float>::ParameterId::kSampleRate, mCurrentSampleRate);
    filter2.setParameter(viator_dsp::SVFilter<float>::ParameterId::kCutoff, 63);
    filter2.setParameter(viator_dsp::SVFilter<float>::ParameterId::kQType, viator_dsp::SVFilter<float>::QType::kProportional);
    
    filter3.prepare(spec);
    filter3.setStereoType(viator_dsp::SVFilter<float>::StereoId::kStereo);
    filter3.setParameter(viator_dsp::SVFilter<float>::ParameterId::kType, viator_dsp::SVFilter<float>::FilterType::kBandShelf);
    filter3.setParameter(viator_dsp::SVFilter<float>::ParameterId::kSampleRate, mCurrentSampleRate);
    filter3.setParameter(viator_dsp::SVFilter<float>::ParameterId::kCutoff, 125);
    filter3.setParameter(viator_dsp::SVFilter<float>::ParameterId::kQType, viator_dsp::SVFilter<float>::QType::kProportional);
    
    filter4.prepare(spec);
    filter4.setStereoType(viator_dsp::SVFilter<float>::StereoId::kStereo);
    filter4.setParameter(viator_dsp::SVFilter<float>::ParameterId::kType, viator_dsp::SVFilter<float>::FilterType::kBandShelf);
    filter4.setParameter(viator_dsp::SVFilter<float>::ParameterId::kSampleRate, mCurrentSampleRate);
    filter4.setParameter(viator_dsp::SVFilter<float>::ParameterId::kCutoff, 250);
    filter4.setParameter(viator_dsp::SVFilter<float>::ParameterId::kQType, viator_dsp::SVFilter<float>::QType::kProportional);
    
    filter5.prepare(spec);
    filter5.setStereoType(viator_dsp::SVFilter<float>::StereoId::kStereo);
    filter5.setParameter(viator_dsp::SVFilter<float>::ParameterId::kType, viator_dsp::SVFilter<float>::FilterType::kBandShelf);
    filter5.setParameter(viator_dsp::SVFilter<float>::ParameterId::kSampleRate, mCurrentSampleRate);
    filter5.setParameter(viator_dsp::SVFilter<float>::ParameterId::kCutoff, 500);
    filter5.setParameter(viator_dsp::SVFilter<float>::ParameterId::kQType, viator_dsp::SVFilter<float>::QType::kProportional);
    
    filter6.prepare(spec);
    filter6.setStereoType(viator_dsp::SVFilter<float>::StereoId::kStereo);
    filter6.setParameter(viator_dsp::SVFilter<float>::ParameterId::kType, viator_dsp::SVFilter<float>::FilterType::kBandShelf);
    filter6.setParameter(viator_dsp::SVFilter<float>::ParameterId::kSampleRate, mCurrentSampleRate);
    filter6.setParameter(viator_dsp::SVFilter<float>::ParameterId::kCutoff, 1000);
    filter6.setParameter(viator_dsp::SVFilter<float>::ParameterId::kQType, viator_dsp::SVFilter<float>::QType::kProportional);
    
    filter7.prepare(spec);
    filter7.setStereoType(viator_dsp::SVFilter<float>::StereoId::kStereo);
    filter7.setParameter(viator_dsp::SVFilter<float>::ParameterId::kType, viator_dsp::SVFilter<float>::FilterType::kBandShelf);
    filter7.setParameter(viator_dsp::SVFilter<float>::ParameterId::kSampleRate, mCurrentSampleRate);
    filter7.setParameter(viator_dsp::SVFilter<float>::ParameterId::kCutoff, 2000);
    filter7.setParameter(viator_dsp::SVFilter<float>::ParameterId::kQType, viator_dsp::SVFilter<float>::QType::kProportional);
    
    filter8.prepare(spec);
    filter8.setStereoType(viator_dsp::SVFilter<float>::StereoId::kStereo);
    filter8.setParameter(viator_dsp::SVFilter<float>::ParameterId::kType, viator_dsp::SVFilter<float>::FilterType::kBandShelf);
    filter8.setParameter(viator_dsp::SVFilter<float>::ParameterId::kSampleRate, mCurrentSampleRate);
    filter8.setParameter(viator_dsp::SVFilter<float>::ParameterId::kCutoff, 4000);
    filter8.setParameter(viator_dsp::SVFilter<float>::ParameterId::kQType, viator_dsp::SVFilter<float>::QType::kProportional);
    
    filter9.prepare(spec);
    filter9.setStereoType(viator_dsp::SVFilter<float>::StereoId::kStereo);
    filter9.setParameter(viator_dsp::SVFilter<float>::ParameterId::kType, viator_dsp::SVFilter<float>::FilterType::kBandShelf);
    filter9.setParameter(viator_dsp::SVFilter<float>::ParameterId::kSampleRate, mCurrentSampleRate);
    filter9.setParameter(viator_dsp::SVFilter<float>::ParameterId::kCutoff, 8000);
    filter9.setParameter(viator_dsp::SVFilter<float>::ParameterId::kQType, viator_dsp::SVFilter<float>::QType::kProportional);
    
    filter10.prepare(spec);
    filter10.setStereoType(viator_dsp::SVFilter<float>::StereoId::kStereo);
    filter10.setParameter(viator_dsp::SVFilter<float>::ParameterId::kType, viator_dsp::SVFilter<float>::FilterType::kBandShelf);
    filter10.setParameter(viator_dsp::SVFilter<float>::ParameterId::kSampleRate, mCurrentSampleRate);
    filter10.setParameter(viator_dsp::SVFilter<float>::ParameterId::kCutoff, 15000);
    filter10.setParameter(viator_dsp::SVFilter<float>::ParameterId::kQType, viator_dsp::SVFilter<float>::QType::kProportional);
    
}

template <typename SampleType>
void GraphicEQ<SampleType>::setParameter(ParameterId parameter, SampleType parameterValue)
{
    using f = viator_dsp::SVFilter<float>::ParameterId;
    switch (parameter)
    {
        case ParameterId::kFilter1Gain: filter1.setParameter(f::kGain, parameterValue); break;
        case ParameterId::kFilter2Gain: filter2.setParameter(f::kGain, parameterValue); break;
        case ParameterId::kFilter3Gain: filter3.setParameter(f::kGain, parameterValue); break;
        case ParameterId::kFilter4Gain: filter4.setParameter(f::kGain, parameterValue); break;
        case ParameterId::kFilter5Gain: filter5.setParameter(f::kGain, parameterValue); break;
        case ParameterId::kFilter6Gain: filter6.setParameter(f::kGain, parameterValue); break;
        case ParameterId::kFilter7Gain: filter7.setParameter(f::kGain, parameterValue); break;
        case ParameterId::kFilter8Gain: filter8.setParameter(f::kGain, parameterValue); break;
        case ParameterId::kFilter9Gain: filter9.setParameter(f::kGain, parameterValue); break;
        case ParameterId::kFilter10Gain: filter10.setParameter(f::kGain, parameterValue); break;
        case ParameterId::kSampleRate: mCurrentSampleRate = parameterValue; break;
        case ParameterId::kBypass: mGlobalBypass = static_cast<bool>(parameterValue);
    }
}

template <typename SampleType>
void GraphicEQ<SampleType>::setStereoType(int newStereoID)
{
    switch (newStereoID)
    {
        case 0:
        {
            filter1.setStereoType(viator_dsp::SVFilter<float>::StereoId::kStereo);
            filter2.setStereoType(viator_dsp::SVFilter<float>::StereoId::kStereo);
            filter3.setStereoType(viator_dsp::SVFilter<float>::StereoId::kStereo);
            filter4.setStereoType(viator_dsp::SVFilter<float>::StereoId::kStereo);
            filter5.setStereoType(viator_dsp::SVFilter<float>::StereoId::kStereo);
            filter6.setStereoType(viator_dsp::SVFilter<float>::StereoId::kStereo);
            filter7.setStereoType(viator_dsp::SVFilter<float>::StereoId::kStereo);
            filter8.setStereoType(viator_dsp::SVFilter<float>::StereoId::kStereo);
            filter9.setStereoType(viator_dsp::SVFilter<float>::StereoId::kStereo);
            filter10.setStereoType(viator_dsp::SVFilter<float>::StereoId::kStereo);
            break;
        }
            
        case 1:
        {
            filter1.setStereoType(viator_dsp::SVFilter<float>::StereoId::kMids);
            filter2.setStereoType(viator_dsp::SVFilter<float>::StereoId::kMids);
            filter3.setStereoType(viator_dsp::SVFilter<float>::StereoId::kMids);
            filter4.setStereoType(viator_dsp::SVFilter<float>::StereoId::kMids);
            filter5.setStereoType(viator_dsp::SVFilter<float>::StereoId::kMids);
            filter6.setStereoType(viator_dsp::SVFilter<float>::StereoId::kMids);
            filter7.setStereoType(viator_dsp::SVFilter<float>::StereoId::kMids);
            filter8.setStereoType(viator_dsp::SVFilter<float>::StereoId::kMids);
            filter9.setStereoType(viator_dsp::SVFilter<float>::StereoId::kMids);
            filter10.setStereoType(viator_dsp::SVFilter<float>::StereoId::kMids);
            break;
        }
            
        case 2:
        {
            filter1.setStereoType(viator_dsp::SVFilter<float>::StereoId::kSides);
            filter2.setStereoType(viator_dsp::SVFilter<float>::StereoId::kSides);
            filter3.setStereoType(viator_dsp::SVFilter<float>::StereoId::kSides);
            filter4.setStereoType(viator_dsp::SVFilter<float>::StereoId::kSides);
            filter5.setStereoType(viator_dsp::SVFilter<float>::StereoId::kSides);
            filter6.setStereoType(viator_dsp::SVFilter<float>::StereoId::kSides);
            filter7.setStereoType(viator_dsp::SVFilter<float>::StereoId::kSides);
            filter8.setStereoType(viator_dsp::SVFilter<float>::StereoId::kSides);
            filter9.setStereoType(viator_dsp::SVFilter<float>::StereoId::kSides);
            filter10.setStereoType(viator_dsp::SVFilter<float>::StereoId::kSides);
            break;
        }
    }
    

}

template class GraphicEQ<float>;
template class GraphicEQ<double>;
