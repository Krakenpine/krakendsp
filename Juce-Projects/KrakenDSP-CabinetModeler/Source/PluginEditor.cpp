
#include "PluginProcessor.h"
#include "PluginEditor.h"

KrakenDSPCabinetModelerAudioProcessorEditor::KrakenDSPCabinetModelerAudioProcessorEditor (KrakenDSPCabinetModelerAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);

    addAndMakeVisible(lengthSlider);
    lengthSlider.setRange(0.0f, 1.0f, 0.00001f);
    lengthSlider.setTextValueSuffix(" Length");
    lengthSlider.addListener(this);
    lengthSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "LENGTH", lengthSlider);
  

}

KrakenDSPCabinetModelerAudioProcessorEditor::~KrakenDSPCabinetModelerAudioProcessorEditor()
{
}

void KrakenDSPCabinetModelerAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (15.0f));
    
    auto fxName = audioProcessor.getFxName();
    g.drawFittedText (fxName, 20, 80, 200, 20, juce::Justification::centred, 1);

    const int textBoxWidth = 180; 
    auto sliderLeft = 20;

    int y = 150;
    
    juce::String lengthText = " Length: " + juce::String(audioProcessor.cabinetModeler.IRlength) + " samples";
    lengthSlider.setTextValueSuffix(lengthText);
    lengthSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, textBoxWidth, 20);
    lengthSlider.setBounds(sliderLeft, y, getWidth() - sliderLeft - 10, 20);
}


void KrakenDSPCabinetModelerAudioProcessorEditor::sliderValueChanged (juce::Slider* slider)
{
    audioProcessor.cabinetModeler.setIRLength(lengthSlider.getValue());
}

void KrakenDSPCabinetModelerAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
