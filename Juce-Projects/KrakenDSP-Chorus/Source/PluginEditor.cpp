
#include "PluginProcessor.h"
#include "PluginEditor.h"

KrakenDSPChorusAudioProcessorEditor::KrakenDSPChorusAudioProcessorEditor (KrakenDSPChorusAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 500);

    addAndMakeVisible(rateSlider);
    rateSlider.setRange(0.0f, 1.0f, 0.00001f);
    rateSlider.setTextValueSuffix(" Length");
    rateSlider.addListener(this);
    rateSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "RATE", rateSlider);

    addAndMakeVisible(depthSlider);
    depthSlider.setRange(0.0f, 1.0f, 0.00001f);
    depthSlider.setTextValueSuffix(" Mix");
    depthSlider.addListener(this);
    depthSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "DEPTH", depthSlider);

    addAndMakeVisible(minDepthSlider);
    minDepthSlider.setRange(0.0f, 1.0f, 0.00001f);
    minDepthSlider.setTextValueSuffix(" Mix");
    minDepthSlider.addListener(this);
    minDepthSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "MINDEPTH", minDepthSlider);

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

KrakenDSPChorusAudioProcessorEditor::~KrakenDSPChorusAudioProcessorEditor()
{
}

void KrakenDSPChorusAudioProcessorEditor::paint (juce::Graphics& g)
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

    auto rateValue = audioProcessor.chorus.getRateNonNormalized();
    juce::String rateText = " Length: " + juce::String(rateValue) + " Hz";
    rateSlider.setTextValueSuffix(rateText);
    rateSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, textBoxWidth, 20);
    rateSlider.setBounds(sliderLeft, y, getWidth() - sliderLeft - 10, 20);

    auto depthValue = audioProcessor.chorus.getDepthNonNormalized();
    juce::String depthText = " Depth: " + juce::String(depthValue) + " ms";
    depthSlider.setTextValueSuffix(depthText);
    depthSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, textBoxWidth, 20);
    depthSlider.setBounds(sliderLeft, y + 50, getWidth() - sliderLeft - 10, 20);

    auto minDepthValue = audioProcessor.chorus.getMinDepthNonNormalized();
    juce::String minDepthText = " Min depth: " + juce::String(minDepthValue) + " ms";
    minDepthSlider.setTextValueSuffix(minDepthText);
    minDepthSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, textBoxWidth, 20);
    minDepthSlider.setBounds(sliderLeft, y + 100, getWidth() - sliderLeft - 10, 20);
    
    auto mixValue = audioProcessor.chorus.getMix();
    juce::String mixText = " Mix: " + juce::String(mixValue);
    mixSlider.setTextValueSuffix(mixText);
    mixSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, textBoxWidth, 20);
    mixSlider.setBounds(sliderLeft, y + 150, getWidth() - sliderLeft - 10, 20);

    auto feedbackValue = audioProcessor.chorus.getFeedback();
    juce::String feedbackText = " Feedback: " + juce::String(feedbackValue);
    feedbackSlider.setTextValueSuffix(feedbackText);
    feedbackSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, textBoxWidth, 20);
    feedbackSlider.setBounds(sliderLeft, y + 200, getWidth() - sliderLeft - 10, 20);
}

void KrakenDSPChorusAudioProcessorEditor::sliderValueChanged (juce::Slider* slider)
{
    audioProcessor.chorus.setRate(rateSlider.getValue());
    audioProcessor.chorus.setDepth(depthSlider.getValue());
    audioProcessor.chorus.setMinDepth(minDepthSlider.getValue());
    audioProcessor.chorus.setMix(mixSlider.getValue());
    audioProcessor.chorus.setFeedback(feedbackSlider.getValue());
    
    repaint();
}

void KrakenDSPChorusAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
