

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"


class KrakenDSPDistortionAudioProcessorEditor  : public juce::AudioProcessorEditor,
    private juce::Slider::Listener
{
public:
    KrakenDSPDistortionAudioProcessorEditor (KrakenDSPDistortionAudioProcessor&);
    ~KrakenDSPDistortionAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:

    void sliderValueChanged(juce::Slider* slider) override;


    juce::Slider typeSlider;
    juce::Label  typeLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> typeSliderAttachment;
    
    juce::Slider controlSlider;
    juce::Label  controlLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> controlSliderAttachment;

    KrakenDSPDistortionAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KrakenDSPDistortionAudioProcessorEditor)
};
