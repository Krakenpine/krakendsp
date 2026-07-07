#pragma once

#include <JuceHeader.h>
#include "../../../MultiFX.hpp"
#include "../../../Tremolo.hpp"


class KrakenDSPTremoloAudioProcessor  : public juce::AudioProcessor, juce::Timer
{
public:
    KrakenDSPTremoloAudioProcessor();
    ~KrakenDSPTremoloAudioProcessor() override;

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
    
    
    
    krakendsp::Tremolo tremolo = krakendsp::Tremolo(1.0f, 1.0f, 44100.0f);
    
    int cunter = 0;

    bool hasUpdatedGui = false;
    
    juce::String getFxName() const
    {
        return juce::String(tremolo.getName());
    }
    
    juce::String getFxType() const
    {
        return juce::String(tremolo.getType().name);
    }
    
    std::vector<krakendsp::FXControl> getFxControls() const
    {
        return tremolo.getControls();
    }
    
    float getControlValue(int index) const
    {
        return tremolo.getControlValue(index);
    }
    
    size_t getTypeCount() const
    {
        return tremolo.getTypes().size();
    }
    
    
    void timerCallback() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KrakenDSPTremoloAudioProcessor)
};
