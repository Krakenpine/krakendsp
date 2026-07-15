

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"


class KrakenDSPDelayAudioProcessorEditor  : public juce::AudioProcessorEditor,
    private juce::Slider::Listener
{
public:
    KrakenDSPDelayAudioProcessorEditor (KrakenDSPDelayAudioProcessor&);
    ~KrakenDSPDelayAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:

    void sliderValueChanged(juce::Slider* slider) override;

    juce::Slider lengthSlider;
    juce::Label  lengthLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lengthSliderAttachment;

    juce::Slider mixSlider;
    juce::Label  mixLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mixSliderAttachment;

    juce::Slider feedbackSlider;
    juce::Label  feedbackLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> feedbackSliderAttachment;

    KrakenDSPDelayAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KrakenDSPDelayAudioProcessorEditor)
};
