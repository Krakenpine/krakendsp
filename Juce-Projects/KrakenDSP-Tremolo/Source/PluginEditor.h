

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"


class KrakenDSPTremoloAudioProcessorEditor  : public juce::AudioProcessorEditor,
    private juce::Slider::Listener
{
public:
    KrakenDSPTremoloAudioProcessorEditor (KrakenDSPTremoloAudioProcessor&);
    ~KrakenDSPTremoloAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:

    void sliderValueChanged(juce::Slider* slider) override;


    juce::Slider typeSlider;
    juce::Label  typeLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> typeSliderAttachment;
    
    juce::Slider depthControlSlider;
    juce::Label  depthControlLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> depthControlSliderAttachment;
    
    juce::Slider rateControlSlider;
    juce::Label  rateControlLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> rateControlSliderAttachment;
    
    juce::Slider hDistControlSlider;
    juce::Label  hDistControlLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> hDistControlSliderAttachment;
    
    juce::Slider vDistControlSlider;
    juce::Label  vDistControlLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> vDistControlSliderAttachment;
    
    juce::Slider smoothingControlSlider;
    juce::Label  smoothingControlLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> smoothingControlSliderAttachment;

    KrakenDSPTremoloAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KrakenDSPTremoloAudioProcessorEditor)
};
