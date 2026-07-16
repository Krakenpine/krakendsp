

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"


class KrakenDSPCabinetModelerAudioProcessorEditor  : public juce::AudioProcessorEditor,
    private juce::Slider::Listener
{
public:
    KrakenDSPCabinetModelerAudioProcessorEditor (KrakenDSPCabinetModelerAudioProcessor&);
    ~KrakenDSPCabinetModelerAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:

    void sliderValueChanged(juce::Slider* slider) override;

    juce::Slider lengthSlider;
    juce::Label  lengthLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lengthSliderAttachment;

    KrakenDSPCabinetModelerAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KrakenDSPCabinetModelerAudioProcessorEditor)
};
