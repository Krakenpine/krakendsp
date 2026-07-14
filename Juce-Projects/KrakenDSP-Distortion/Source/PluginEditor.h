

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

    juce::Slider oversampleSlider;
    juce::Label  oversampleLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> oversampleSliderAttachment;

    juce::Slider oversampleInterpolationSlider;
    juce::Label  oversampleInterpolationLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> oversampleInterpolationSliderAttachment;

    juce::Slider filter1FreqSlider;
    juce::Label  filter1FreqLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filter1FreqSliderAttachment;

    juce::Slider filter1TypeSlider;
    juce::Label  filter1TypeLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filter1TypeSliderAttachment;

    juce::Slider filter1GainSlider;
    juce::Label  filter1GainLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filter1GainSliderAttachment;

    juce::Slider filter1QSlider;
    juce::Label  filter1QLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filter1QSliderAttachment;

    KrakenDSPDistortionAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KrakenDSPDistortionAudioProcessorEditor)
};
