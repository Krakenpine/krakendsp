#include "PluginProcessor.h"
#include "PluginEditor.h"

KrakenDSPDistortionAudioProcessor::KrakenDSPDistortionAudioProcessor(): apvts(*this, nullptr, juce::Identifier ("KrakenDSP Distortion"),
{
    std::make_unique<juce::AudioParameterFloat>("TYPESELECT", "Type select", 0.0f, 1.0f, 0.2f),
    std::make_unique<juce::AudioParameterFloat>("CONTROL0", "Control 0", 0.0f, 1.0f, 0.000001f),
    std::make_unique<juce::AudioParameterFloat>("OVERSAMPLE", "Oversample", 0.0f, 1.0f, 0.1f),
    std::make_unique<juce::AudioParameterFloat>("OSINTERPOLATION", "Oversample interpolation mode", 0.0f, 1.0f, 0.1f),
    std::make_unique<juce::AudioParameterFloat>("FILTER1FREQ", "Filter 1 freq", 0.0f, 1.0f, 0.0001f),
    std::make_unique<juce::AudioParameterFloat>("FILTER1TYPE", "Filter 1 type", 0.0f, 1.0f, 0.0001f),
    std::make_unique<juce::AudioParameterFloat>("FILTER1GAIN", "Filter 1 gain", 0.0f, 1.0f, 0.0001f),
    std::make_unique<juce::AudioParameterFloat>("FILTER1Q", "Filter 1 q", 0.0f, 1.0f, 0.0001f)
})
{
    juce::Timer::startTimerHz(30); // Timer for GUI updates
}

void KrakenDSPDistortionAudioProcessor::timerCallback()
{
    if (auto* editor = getActiveEditor())
    {
        if (!hasUpdatedGui){
            //hasUpdatedGui = true;
            editor->repaint(); // Request a repaint of the editor
        }
    }
}


KrakenDSPDistortionAudioProcessor::~KrakenDSPDistortionAudioProcessor()
{
}

//==============================================================================
const juce::String KrakenDSPDistortionAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool KrakenDSPDistortionAudioProcessor::acceptsMidi() const
{
    return false;
}

bool KrakenDSPDistortionAudioProcessor::producesMidi() const
{
    return false;
}

bool KrakenDSPDistortionAudioProcessor::isMidiEffect() const
{
    return false;
}

double KrakenDSPDistortionAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int KrakenDSPDistortionAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int KrakenDSPDistortionAudioProcessor::getCurrentProgram()
{
    return 0;
}

void KrakenDSPDistortionAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String KrakenDSPDistortionAudioProcessor::getProgramName (int index)
{
    return {};
}

void KrakenDSPDistortionAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void KrakenDSPDistortionAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void KrakenDSPDistortionAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool KrakenDSPDistortionAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void KrakenDSPDistortionAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{

    if (hasLoadedState && !hasLoaded) {
        hasLoaded = true;
        
        auto* parameterGain = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter("CONTROL0"));
        if (parameterGain) {
            parameterGain->setValueNotifyingHost(parameterGain->convertTo0to1(distortion.getControlValueRaw(0)));
        }
            
        auto* parameterType = apvts.getParameter("TYPESELECT");
        if (parameterType) {
            parameterType->setValueNotifyingHost(parameterType->convertTo0to1(static_cast<float>(distortion.getType().typeID)/static_cast<float>(distortion.getTypes().size()-1)));
        }

        auto* parameterFilterFreq = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter("FILTER1FREQ"));
        if (parameterFilterFreq) {
            parameterFilterFreq->setValueNotifyingHost(parameterFilterFreq->convertTo0to1(distortion.getFilter1Freq()));
        }

        auto* parameterFilterGain = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter("FILTER1GAIN"));
        if (parameterFilterGain) {
            parameterFilterGain->setValueNotifyingHost(parameterFilterGain->convertTo0to1(distortion.getFilter1Gain()));
        }

        auto* parameterFilterQ = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter("FILTER1Q"));
        if (parameterFilterQ) {
            parameterFilterQ->setValueNotifyingHost(parameterFilterQ->convertTo0to1(distortion.getFilter1Q()));
        }

        auto* parameterFilterType = apvts.getParameter("FILTER1TYPE");
        if (parameterFilterType) {
            parameterFilterType->setValueNotifyingHost(parameterFilterType->convertTo0to1(static_cast<float>(distortion.getFilter1Type().typeID)/static_cast<float>(distortion.getTypes().size()-1)));
        }

        if (auto* editor = getActiveEditor())
        {
            editor->repaint(); // Request a repaint of the editor
        }

    }

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    
    for (auto sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
        auto *inBufferLeft = buffer.getReadPointer(0);
        auto *inBufferRight = buffer.getReadPointer(1);
        
        auto *outBufferLeft = buffer.getWritePointer(0);
        auto *outBufferRight = buffer.getWritePointer(1);
        
        krakendsp::Stereo input;
        input.left = inBufferLeft[sample];
        input.right = inBufferRight[sample];
        
        krakendsp::Stereo output;
        output = distortion.process(input);
        
        outBufferLeft[sample] = output.left;
        outBufferRight[sample] = output.right;
    }

}

//==============================================================================
bool KrakenDSPDistortionAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* KrakenDSPDistortionAudioProcessor::createEditor()
{
    return new KrakenDSPDistortionAudioProcessorEditor (*this);
}

void KrakenDSPDistortionAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    destData.setSize(0);
    float gain = distortion.getControlValueRaw(0);
    int typeID = distortion.getType().typeID;
    
    float filterFreq = distortion.getFilter1Freq();
    float filterGain = distortion.getFilter1Gain();
    float filterQ = distortion.getFilter1Q();
    int filterType = distortion.getFilter1Type().typeID;
    
    unsigned int memplace = 0;
    destData.insert(&gain, sizeof(float), 0);
    memplace += sizeof(float);
    destData.insert(&typeID, sizeof(int), memplace);
    memplace += sizeof(int);
    destData.insert(&filterFreq, sizeof(float), memplace);
    memplace += sizeof(float);
    destData.insert(&filterGain, sizeof(float), memplace);
    memplace += sizeof(float);
    destData.insert(&filterQ, sizeof(float), memplace);
    memplace += sizeof(float);
    destData.insert(&filterType, sizeof(int), memplace);
}

void KrakenDSPDistortionAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    float gain;
    int typeID;
    
    float filterFreq;
    float filterGain;
    float filterQ;
    int filterType;
    
    unsigned int memplace = 0;
    std::memcpy(&gain, data, sizeof(float));
    memplace += sizeof(float);
    std::memcpy(&typeID, static_cast<const char*>(data) + memplace, sizeof(int));
    memplace += sizeof(int);
    std::memcpy(&filterFreq, static_cast<const char*>(data) + memplace, sizeof(float));
    memplace += sizeof(float);
    std::memcpy(&filterGain, static_cast<const char*>(data) + memplace, sizeof(float));
    memplace += sizeof(float);
    std::memcpy(&filterQ, static_cast<const char*>(data) + memplace, sizeof(float));
    memplace += sizeof(float);
    std::memcpy(&filterType, static_cast<const char*>(data) + memplace, sizeof(int));

    distortion.setControl(0, gain);
    distortion.setType(typeID);
    
    
    distortion.setFilter1Freq(6666);
    distortion.setFilter1Gain(0.5);
    distortion.setFilter1Q(filterQ);    
    distortion.setFilter1Type(3);

    hasLoadedState = true;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new KrakenDSPDistortionAudioProcessor();
}
