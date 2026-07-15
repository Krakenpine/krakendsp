#include "PluginProcessor.h"
#include "PluginEditor.h"

KrakenDSPChorusAudioProcessor::KrakenDSPChorusAudioProcessor(): apvts(*this, nullptr, juce::Identifier ("KrakenDSP Chorus"),
{
    std::make_unique<juce::AudioParameterFloat>("RATE", "Rate", 0.0f, 1.0f, 0.0001f),
    std::make_unique<juce::AudioParameterFloat>("DEPTH", "Depth", 0.0f, 1.0f, 0.0001f),
    std::make_unique<juce::AudioParameterFloat>("MINDEPTH", "Min depth", 0.0f, 1.0f, 0.0001f),
    std::make_unique<juce::AudioParameterFloat>("MIX", "Mix", 0.0f, 1.0f, 0.0001f),
    std::make_unique<juce::AudioParameterFloat>("FEEDBACK", "Feedback", 0.0f, 1.0f, 0.0001f),

})
{
    juce::Timer::startTimerHz(30); // Timer for GUI updates
}

void KrakenDSPChorusAudioProcessor::timerCallback()
{
    if (auto* editor = getActiveEditor())
    {
        if (!hasUpdatedGui){
            //hasUpdatedGui = true;
            editor->repaint(); // Request a repaint of the editor
        }
    }
}


KrakenDSPChorusAudioProcessor::~KrakenDSPChorusAudioProcessor()
{
}

//==============================================================================
const juce::String KrakenDSPChorusAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool KrakenDSPChorusAudioProcessor::acceptsMidi() const
{
    return false;
}

bool KrakenDSPChorusAudioProcessor::producesMidi() const
{
    return false;
}

bool KrakenDSPChorusAudioProcessor::isMidiEffect() const
{
    return false;
}

double KrakenDSPChorusAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int KrakenDSPChorusAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int KrakenDSPChorusAudioProcessor::getCurrentProgram()
{
    return 0;
}

void KrakenDSPChorusAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String KrakenDSPChorusAudioProcessor::getProgramName (int index)
{
    return {};
}

void KrakenDSPChorusAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void KrakenDSPChorusAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void KrakenDSPChorusAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool KrakenDSPChorusAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void KrakenDSPChorusAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
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
        output.left = chorus.process(input.left);
        
        outBufferLeft[sample] = output.left;
        outBufferRight[sample] = output.left;
    }

}

//==============================================================================
bool KrakenDSPChorusAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* KrakenDSPChorusAudioProcessor::createEditor()
{
    return new KrakenDSPChorusAudioProcessorEditor (*this);
}

void KrakenDSPChorusAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{

}

void KrakenDSPChorusAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    hasLoadedState = true;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new KrakenDSPChorusAudioProcessor();
}
