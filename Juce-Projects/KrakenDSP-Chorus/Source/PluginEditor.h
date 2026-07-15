

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"


class KrakenDSPChorusAudioProcessorEditor  : public juce::AudioProcessorEditor,
    private juce::Slider::Listener
{
public:
    KrakenDSPChorusAudioProcessorEditor (KrakenDSPChorusAudioProcessor&);
    ~KrakenDSPChorusAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:

    void sliderValueChanged(juce::Slider* slider) override;

    juce::Slider rateSlider;
    juce::Label  rateLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> rateSliderAttachment;

    juce::Slider depthSlider;
    juce::Label  depthLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> depthSliderAttachment;

    juce::Slider minDepthSlider;
    juce::Label  minDepthLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> minDepthSliderAttachment;

    juce::Slider mixSlider;
    juce::Label  mixLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mixSliderAttachment;

    juce::Slider feedbackSlider;
    juce::Label  feedbackLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> feedbackSliderAttachment;

    KrakenDSPChorusAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KrakenDSPChorusAudioProcessorEditor)
};
