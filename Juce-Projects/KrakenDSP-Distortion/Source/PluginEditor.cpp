
#include "PluginProcessor.h"
#include "PluginEditor.h"

KrakenDSPDistortionAudioProcessorEditor::KrakenDSPDistortionAudioProcessorEditor (KrakenDSPDistortionAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 500);
    
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

    addAndMakeVisible(oversampleSlider);
    oversampleSlider.setRange(0.0f, 1.0f, 0.1f);
    oversampleSlider.setTextValueSuffix(" Oversample");
    oversampleSlider.addListener(this);
    oversampleSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "OVERSAMPLE", oversampleSlider);
    
    addAndMakeVisible(oversampleInterpolationSlider);
    oversampleInterpolationSlider.setRange(0.0f, 1.0f, 0.1f);
    oversampleInterpolationSlider.setTextValueSuffix(" Oversample");
    oversampleInterpolationSlider.addListener(this);
    oversampleInterpolationSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "OSINTERPOLATION", oversampleInterpolationSlider);

    addAndMakeVisible(filter1FreqSlider);
    filter1FreqSlider.setRange(0.0f, 1.0f, 0.00001f);
    filter1FreqSlider.setTextValueSuffix(" Filt1 Freq");
    filter1FreqSlider.addListener(this);
    filter1FreqSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "FILTER1FREQ", filter1FreqSlider);

    addAndMakeVisible(filter1TypeSlider);
    filter1TypeSlider.setRange(0.0f, 1.0f, 0.2f);
    filter1TypeSlider.setTextValueSuffix(" Filt1 type");
    filter1TypeSlider.addListener(this);
    filter1TypeSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "FILTER1TYPE", filter1TypeSlider);

    addAndMakeVisible(filter1GainSlider);
    filter1GainSlider.setRange(0.0f, 1.0f, 0.00001f);
    filter1GainSlider.setTextValueSuffix(" Filt1 Freq");
    filter1GainSlider.addListener(this);
    filter1GainSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "FILTER1GAIN", filter1GainSlider);

    addAndMakeVisible(filter1QSlider);
    filter1QSlider.setRange(0.0f, 1.0f, 0.00001f);
    filter1QSlider.setTextValueSuffix(" Filt1 Freq");
    filter1QSlider.addListener(this);
    filter1QSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "FILTER1Q", filter1QSlider);


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

    auto oversampleValue = audioProcessor.distortion.getOversample();
    juce::String oversampleText = " Oversample: " + juce::String(oversampleValue);
    oversampleSlider.setTextValueSuffix(oversampleText);
    oversampleSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, textBoxWidth, 20);
    oversampleSlider.setBounds(sliderLeft, y + 50, getWidth() - sliderLeft - 10, 20);

    auto oversampleInterpolationValue = audioProcessor.distortion.getInterpolationUpsampleMode();
    juce::String oversampleInterpolationText = " Upsample mode: filtered";
    if (oversampleInterpolationValue) {
        oversampleInterpolationText = " Upsample mode: interpolated";
    }
    oversampleInterpolationSlider.setTextValueSuffix(oversampleInterpolationText);
    oversampleInterpolationSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, textBoxWidth, 20);
    oversampleInterpolationSlider.setBounds(sliderLeft, y + 100, getWidth() - sliderLeft - 10, 20);

    auto filter1FreqValue = audioProcessor.distortion.getFilter1Freq();
    juce::String filter1FreqText = " Filter 1 Freq: " + juce::String(filter1FreqValue, 2);
    filter1FreqSlider.setTextValueSuffix(filter1FreqText);
    filter1FreqSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, textBoxWidth, 20);
    filter1FreqSlider.setBounds(sliderLeft, y + 150, getWidth() - sliderLeft - 10, 20);

    juce::String filter1TypeText = " Filter 1 type" + audioProcessor.distortion.getFilter1Type();
    filter1TypeSlider.setTextValueSuffix(filter1TypeText);
    filter1TypeSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, textBoxWidth, 20);
    filter1TypeSlider.setBounds(sliderLeft, y + 200, getWidth() - sliderLeft - 10, 20);

    auto filter1Gain = audioProcessor.distortion.getFilter1Gain();
    juce::String filter1GainText = " Filter 1 gain: " + juce::String(filter1Gain, 2);
    filter1GainSlider.setTextValueSuffix(filter1GainText);
    filter1GainSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, textBoxWidth, 20);
    filter1GainSlider.setBounds(sliderLeft, y + 250, getWidth() - sliderLeft - 10, 20);

    auto filter1Q = audioProcessor.distortion.getFilter1Q();
    juce::String filter1QText = " Filter 1 q: " + juce::String(filter1Q, 2);
    filter1QSlider.setTextValueSuffix(filter1QText);
    filter1QSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, textBoxWidth, 20);
    filter1QSlider.setBounds(sliderLeft, y + 300, getWidth() - sliderLeft - 10, 20);
}


void KrakenDSPDistortionAudioProcessorEditor::sliderValueChanged (juce::Slider* slider)
{
    audioProcessor.distortion.setType(static_cast<int>(0.001f + typeSlider.getValue() * static_cast<float>(audioProcessor.distortion.getTypes().size() - 1)));
    
    audioProcessor.distortion.setControl(0, controlSlider.getValue());

    audioProcessor.distortion.setOversample(static_cast<size_t>((oversampleSlider.getValue() + 0.001f) * 7.0f) + 1.0f);

    audioProcessor.distortion.setInterpolationUpsampleMode(oversampleInterpolationSlider.getValue() > 0.5f);

    audioProcessor.distortion.setFilter1Freq(filter1FreqSlider.getValue() * 10000.0f);

    audioProcessor.distortion.setFilter1Type(static_cast<size_t>((filter1TypeSlider.getValue() + 0.001f) * 5.0f));

    audioProcessor.distortion.setFilter1Gain((filter1GainSlider.getValue() - 0.5f) * 60.0f);

    float qTemp = filter1QSlider.getValue() * 2.0f;

    float q = 0.0f;

    if (qTemp >= 1.0f) {
        q = 1.0f + (qTemp - 1.0f) * 9.0f;
    } else {
        q = qTemp * 0.9f + 0.1f;
    }

    audioProcessor.distortion.setFilter1Q(q);
    
    repaint();
}

void KrakenDSPDistortionAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
