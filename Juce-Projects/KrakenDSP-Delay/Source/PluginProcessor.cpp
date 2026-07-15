#include "PluginProcessor.h"
#include "PluginEditor.h"

KrakenDSPDelayAudioProcessor::KrakenDSPDelayAudioProcessor(): apvts(*this, nullptr, juce::Identifier ("KrakenDSP Delay"),
{
    std::make_unique<juce::AudioParameterFloat>("LENGTH", "Length", 0.0f, 1.0f, 0.0001f),
    std::make_unique<juce::AudioParameterFloat>("MIX", "Mix", 0.0f, 1.0f, 0.0001f),
    std::make_unique<juce::AudioParameterFloat>("FEEDBACK", "Feedback", 0.0f, 1.0f, 0.0001f),

})
{
    juce::Timer::startTimerHz(30); // Timer for GUI updates
}

void KrakenDSPDelayAudioProcessor::timerCallback()
{
    if (auto* editor = getActiveEditor())
    {
        if (!hasUpdatedGui){
            //hasUpdatedGui = true;
            editor->repaint(); // Request a repaint of the editor
        }
    }
}


KrakenDSPDelayAudioProcessor::~KrakenDSPDelayAudioProcessor()
{
}

//==============================================================================
const juce::String KrakenDSPDelayAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool KrakenDSPDelayAudioProcessor::acceptsMidi() const
{
    return false;
}

bool KrakenDSPDelayAudioProcessor::producesMidi() const
{
    return false;
}

bool KrakenDSPDelayAudioProcessor::isMidiEffect() const
{
    return false;
}

double KrakenDSPDelayAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int KrakenDSPDelayAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int KrakenDSPDelayAudioProcessor::getCurrentProgram()
{
    return 0;
}

void KrakenDSPDelayAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String KrakenDSPDelayAudioProcessor::getProgramName (int index)
{
    return {};
}

void KrakenDSPDelayAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void KrakenDSPDelayAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void KrakenDSPDelayAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool KrakenDSPDelayAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void KrakenDSPDelayAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{

    if (hasLoadedState && !hasLoaded) {
        hasLoaded = true;

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
        output.left = delay.process(input.left);
        
        outBufferLeft[sample] = output.left;
        outBufferRight[sample] = output.left;
    }

}

//==============================================================================
bool KrakenDSPDelayAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* KrakenDSPDelayAudioProcessor::createEditor()
{
    return new KrakenDSPDelayAudioProcessorEditor (*this);
}

void KrakenDSPDelayAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{

}

void KrakenDSPDelayAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    hasLoadedState = true;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new KrakenDSPDelayAudioProcessor();
}
