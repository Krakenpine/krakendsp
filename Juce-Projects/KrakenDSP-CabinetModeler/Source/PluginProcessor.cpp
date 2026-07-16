#include "PluginProcessor.h"
#include "PluginEditor.h"

KrakenDSPCabinetModelerAudioProcessor::KrakenDSPCabinetModelerAudioProcessor(): apvts(*this, nullptr, juce::Identifier ("KrakenDSP CabinetModeler"),
{
    std::make_unique<juce::AudioParameterFloat>("LENGTH", "Length", 0.0f, 1.0f, 0.0001f)
})
{
    juce::Timer::startTimerHz(30); // Timer for GUI updates
}

void KrakenDSPCabinetModelerAudioProcessor::timerCallback()
{
    if (auto* editor = getActiveEditor())
    {
        if (!hasUpdatedGui){
            //hasUpdatedGui = true;
            editor->repaint(); // Request a repaint of the editor
        }
    }
}


KrakenDSPCabinetModelerAudioProcessor::~KrakenDSPCabinetModelerAudioProcessor()
{
}

//==============================================================================
const juce::String KrakenDSPCabinetModelerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool KrakenDSPCabinetModelerAudioProcessor::acceptsMidi() const
{
    return false;
}

bool KrakenDSPCabinetModelerAudioProcessor::producesMidi() const
{
    return false;
}

bool KrakenDSPCabinetModelerAudioProcessor::isMidiEffect() const
{
    return false;
}

double KrakenDSPCabinetModelerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int KrakenDSPCabinetModelerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int KrakenDSPCabinetModelerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void KrakenDSPCabinetModelerAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String KrakenDSPCabinetModelerAudioProcessor::getProgramName (int index)
{
    return {};
}

void KrakenDSPCabinetModelerAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void KrakenDSPCabinetModelerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void KrakenDSPCabinetModelerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool KrakenDSPCabinetModelerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void KrakenDSPCabinetModelerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
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
        output = cabinetModeler.process(input);
        
        outBufferLeft[sample] = output.left;
        outBufferRight[sample] = output.right;
    }

}

//==============================================================================
bool KrakenDSPCabinetModelerAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* KrakenDSPCabinetModelerAudioProcessor::createEditor()
{
    return new KrakenDSPCabinetModelerAudioProcessorEditor (*this);
}

//==============================================================================
void KrakenDSPCabinetModelerAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void KrakenDSPCabinetModelerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new KrakenDSPCabinetModelerAudioProcessor();
}
