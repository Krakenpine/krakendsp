#include "PluginProcessor.h"
#include "PluginEditor.h"

KrakenDSPTremoloAudioProcessor::KrakenDSPTremoloAudioProcessor(): apvts(*this, nullptr, juce::Identifier ("KrakenDSP Tremolo"),
{
    std::make_unique<juce::AudioParameterFloat>("TYPESELECT", "Type select", 0.0f, 1.0f, 0.01f),
    std::make_unique<juce::AudioParameterFloat>("CONTROL0", "Control 0", 0.0f, 1.0f, 0.000001f),
    std::make_unique<juce::AudioParameterFloat>("CONTROL1", "Control 1", 0.0f, 1.0f, 0.000001f),
    std::make_unique<juce::AudioParameterFloat>("CONTROL2", "Control 2", 0.0f, 1.0f, 0.000001f),
    std::make_unique<juce::AudioParameterFloat>("CONTROL3", "Control 3", 0.0f, 1.0f, 0.000001f),
    std::make_unique<juce::AudioParameterFloat>("CONTROL4", "Control 4", 0.0f, 1.0f, 0.000001f),
})
{
    juce::Timer::startTimerHz(30); // Timer for GUI updates
}

void KrakenDSPTremoloAudioProcessor::timerCallback()
{
    cunter++;
    if (auto* editor = getActiveEditor())
    {
        if (!hasUpdatedGui){
            //hasUpdatedGui = true;
            editor->repaint(); // Request a repaint of the editor
        }
    }
}


KrakenDSPTremoloAudioProcessor::~KrakenDSPTremoloAudioProcessor()
{
}

//==============================================================================
const juce::String KrakenDSPTremoloAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool KrakenDSPTremoloAudioProcessor::acceptsMidi() const
{
    return false;
}

bool KrakenDSPTremoloAudioProcessor::producesMidi() const
{
    return false;
}

bool KrakenDSPTremoloAudioProcessor::isMidiEffect() const
{
    return false;
}

double KrakenDSPTremoloAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int KrakenDSPTremoloAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int KrakenDSPTremoloAudioProcessor::getCurrentProgram()
{
    return 0;
}

void KrakenDSPTremoloAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String KrakenDSPTremoloAudioProcessor::getProgramName (int index)
{
    return {};
}

void KrakenDSPTremoloAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void KrakenDSPTremoloAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void KrakenDSPTremoloAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool KrakenDSPTremoloAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void KrakenDSPTremoloAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
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
        output = tremolo.process(input);
        
        outBufferLeft[sample] = output.left;
        outBufferRight[sample] = output.right;
    }

}

//==============================================================================
bool KrakenDSPTremoloAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* KrakenDSPTremoloAudioProcessor::createEditor()
{
    return new KrakenDSPTremoloAudioProcessorEditor (*this);
}

//==============================================================================
void KrakenDSPTremoloAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void KrakenDSPTremoloAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new KrakenDSPTremoloAudioProcessor();
}
