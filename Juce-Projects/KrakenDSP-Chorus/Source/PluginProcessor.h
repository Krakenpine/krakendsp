#pragma once

#include <JuceHeader.h>
#include "../../../MultiFX.hpp"
#include "../../../Chorus.hpp"


class KrakenDSPChorusAudioProcessor  : public juce::AudioProcessor, juce::Timer
{
public:
    KrakenDSPChorusAudioProcessor();
    ~KrakenDSPChorusAudioProcessor() override;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    juce::AudioProcessorValueTreeState apvts;
    
    krakendsp::Chorus chorus = krakendsp::Chorus(0.3f, 0.5f, 0.1f, 0.5f, 0.0f, 44100.0f, 88000, nullptr, nullptr, false);

    bool hasUpdatedGui = false;
    bool hasLoaded = false;
    bool hasLoadedState = false;
    
    juce::String getFxName() const
    {
        return juce::String(chorus.getName());
    }
    
    juce::String getFxType() const
    {
        return juce::String(chorus.getType().name);
    }
    
    std::vector<krakendsp::FXControl> getFxControls() const
    {
        return chorus.getControls();
    }
    
    float getControlValue(int index) const
    {
        return chorus.getControlValue(index);
    }
    
    size_t getTypeCount() const
    {
        return chorus.getTypes().size();
    }
    
    
    void timerCallback() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KrakenDSPChorusAudioProcessor)
};
