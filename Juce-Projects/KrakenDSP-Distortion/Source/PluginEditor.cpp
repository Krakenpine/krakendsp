
#include "PluginProcessor.h"
#include "PluginEditor.h"

KrakenDSPDistortionAudioProcessorEditor::KrakenDSPDistortionAudioProcessorEditor (KrakenDSPDistortionAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    
    addAndMakeVisible(typeSlider);
    typeSlider.setRange(0.0f, 1.0f, 0.2f);
    typeSlider.setTextValueSuffix(" Type Select");
    typeSlider.addListener(this);
    typeSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "TYPESELECT", typeSlider);

    addAndMakeVisible(controlSlider);
    controlSlider.setRange(0.0f, 1.0f, 0.00001f);
    controlSlider.setTextValueSuffix(" Control");
    controlSlider.addListener(this);
    controlSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "CONTROL0", controlSlider);
    
}

KrakenDSPDistortionAudioProcessorEditor::~KrakenDSPDistortionAudioProcessorEditor()
{
}

void KrakenDSPDistortionAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (15.0f));
    //g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
    
    auto fxName = audioProcessor.getFxName();
    g.drawFittedText (fxName, 20, 80, 200, 20, juce::Justification::centred, 1);
    auto fxType = audioProcessor.getFxType();
    g.drawFittedText (fxType, 20, 110, 200, 20, juce::Justification::centred, 1);
    
    auto sliderLeft = 20;
    typeSlider.setBounds(sliderLeft, 50, getWidth() - sliderLeft - 10, 20);
    
    int y = 150;
    
    auto controls = audioProcessor.getFxControls();
    
    auto distControl = controls[0];
    
    auto distValue = audioProcessor.getControlValue(distControl.index) * distControl.displayMultiplier;
    juce::String controlText = " " + juce::String(distControl.name) + ": " + juce::String(distValue, 2) + " " +  juce::String(distControl.unit);

    controlSlider.setTextValueSuffix(controlText);
    const int textBoxWidth = 180;
    controlSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, textBoxWidth, 20);
    controlSlider.setBounds(sliderLeft, y, getWidth() - sliderLeft - 10, 20);

}


void KrakenDSPDistortionAudioProcessorEditor::sliderValueChanged (juce::Slider* slider)
{
    audioProcessor.distortion.setType(static_cast<int>(0.001f + typeSlider.getValue() * static_cast<float>(audioProcessor.distortion.getTypes().size() - 1)));
    
    audioProcessor.distortion.setControl(0, controlSlider.getValue());
    
    repaint();
}

void KrakenDSPDistortionAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
