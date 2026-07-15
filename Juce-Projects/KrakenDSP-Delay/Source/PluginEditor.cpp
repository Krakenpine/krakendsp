
#include "PluginProcessor.h"
#include "PluginEditor.h"

KrakenDSPDelayAudioProcessorEditor::KrakenDSPDelayAudioProcessorEditor (KrakenDSPDelayAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 500);

    addAndMakeVisible(lengthSlider);
    lengthSlider.setRange(0.0f, 1.0f, 0.00001f);
    lengthSlider.setTextValueSuffix(" Length");
    lengthSlider.addListener(this);
    lengthSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "LENGTH", lengthSlider);

    addAndMakeVisible(mixSlider);
    mixSlider.setRange(0.0f, 1.0f, 0.00001f);
    mixSlider.setTextValueSuffix(" Mix");
    mixSlider.addListener(this);
    mixSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "MIX", mixSlider);

    addAndMakeVisible(feedbackSlider);
    feedbackSlider.setRange(0.0f, 1.0f, 0.00001f);
    feedbackSlider.setTextValueSuffix(" Feedback");
    feedbackSlider.addListener(this);
    feedbackSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "FEEDBACK", feedbackSlider);
}

KrakenDSPDelayAudioProcessorEditor::~KrakenDSPDelayAudioProcessorEditor()
{
}

void KrakenDSPDelayAudioProcessorEditor::paint (juce::Graphics& g)
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

    const int textBoxWidth = 180;
    auto sliderLeft = 20;

    int y = 150;
    
    auto controls = audioProcessor.getFxControls();

    auto lengthValue = audioProcessor.delay.getLengthMS();
    juce::String lengthText = " Length: " + juce::String(lengthValue) + " ms";
    lengthSlider.setTextValueSuffix(lengthText);
    lengthSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, textBoxWidth, 20);
    lengthSlider.setBounds(sliderLeft, y, getWidth() - sliderLeft - 10, 20);
    
    auto mixValue = audioProcessor.delay.getMix();
    juce::String mixText = " Mix: " + juce::String(mixValue);
    mixSlider.setTextValueSuffix(mixText);
    mixSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, textBoxWidth, 20);
    mixSlider.setBounds(sliderLeft, y + 50, getWidth() - sliderLeft - 10, 20);

    auto feedbackValue = audioProcessor.delay.getFeedback();
    juce::String feedbackText = " Feedback: " + juce::String(feedbackValue);
    feedbackSlider.setTextValueSuffix(feedbackText);
    feedbackSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, textBoxWidth, 20);
    feedbackSlider.setBounds(sliderLeft, y + 100, getWidth() - sliderLeft - 10, 20);
}


void KrakenDSPDelayAudioProcessorEditor::sliderValueChanged (juce::Slider* slider)
{

    audioProcessor.delay.setLengthNormalized(lengthSlider.getValue());
    audioProcessor.delay.setMix(mixSlider.getValue());
    audioProcessor.delay.setFeedback(feedbackSlider.getValue());
    
    repaint();
}

void KrakenDSPDelayAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
