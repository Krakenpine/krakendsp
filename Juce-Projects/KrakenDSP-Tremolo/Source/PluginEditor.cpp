
#include "PluginProcessor.h"
#include "PluginEditor.h"

KrakenDSPTremoloAudioProcessorEditor::KrakenDSPTremoloAudioProcessorEditor (KrakenDSPTremoloAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    
    addAndMakeVisible(typeSlider);
    typeSlider.setRange(0, 1.0f, 0.01f);
    typeSlider.setTextValueSuffix(" Type Select");
    typeSlider.addListener(this);
    typeSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "TYPESELECT", typeSlider);

    addAndMakeVisible(depthControlSlider);
    depthControlSlider.setRange(0.0f, 1.0f, 0.00001f);
    depthControlSlider.setTextValueSuffix(" Control");
    depthControlSlider.addListener(this);
    depthControlSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "CONTROL0", depthControlSlider);
    
    addAndMakeVisible(rateControlSlider);
    rateControlSlider.setRange(0.0f, 1.0f, 0.00001f);
    rateControlSlider.setTextValueSuffix(" Control");
    rateControlSlider.addListener(this);
    rateControlSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "CONTROL1", rateControlSlider);
    
}

KrakenDSPTremoloAudioProcessorEditor::~KrakenDSPTremoloAudioProcessorEditor()
{
}

void KrakenDSPTremoloAudioProcessorEditor::paint (juce::Graphics& g)
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
    
    auto depthControl = controls[0];
    
    auto depthValue = audioProcessor.getControlValue(depthControl.index) * depthControl.displayMultiplier;
    juce::String depthControlText = " " + juce::String(depthControl.name) + ": " + juce::String(depthValue, 2) + " " +  juce::String(depthControl.unit);

    depthControlSlider.setTextValueSuffix(depthControlText);
    const int textBoxWidth = 180;
    depthControlSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, textBoxWidth, 20);
    depthControlSlider.setBounds(sliderLeft, y, getWidth() - sliderLeft - 10, 20);
    
    y += 30;
    
    auto rateControl = controls[1];
    
    auto rateValue = audioProcessor.getControlValue(rateControl.index) * rateControl.displayMultiplier;
    juce::String rateControlText = " " + juce::String(rateControl.name) + ": " + juce::String(rateValue, 2) + " " +  juce::String(rateControl.unit);

    rateControlSlider.setTextValueSuffix(rateControlText);

    rateControlSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, textBoxWidth, 20);
    rateControlSlider.setBounds(sliderLeft, y, getWidth() - sliderLeft - 10, 20);

}


void KrakenDSPTremoloAudioProcessorEditor::sliderValueChanged (juce::Slider* slider)
{
    typeSlider.setRange(0, audioProcessor.getTypeCount());
    
    audioProcessor.tremolo.setType(static_cast<int>(typeSlider.getValue()));
    
    audioProcessor.tremolo.setControl(0, depthControlSlider.getValue());
    audioProcessor.tremolo.setControl(1, rateControlSlider.getValue());
    
    repaint();
}

void KrakenDSPTremoloAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
