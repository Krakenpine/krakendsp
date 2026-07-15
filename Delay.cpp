#include "Delay.hpp"
#include "RingBuffer.hpp"
#include <algorithm>

krakendsp::Delay::Delay(float feedback_, float mix_, float length_, float samplerate_, size_t bufferSize_, bool isMono_, bool dynamicBuffer_, float* buffer1_, float* buffer2_) :
    feedback(feedback_),
    mix(mix_),
    delayLengthNormalized(length_),
    samplerate(samplerate_),
    bufferSize(bufferSize_),
    isMono(isMono_),
    dynamicBuffer(dynamicBuffer_),
    buffer1(buffer1_),
    buffer2(buffer2_)
    {
        controls[2].displayMultiplier = samplesToMS(static_cast<float>(bufferSize));
        controls[2].valueMax = samplesToMS(static_cast<float>(bufferSize));

        if (dynamicBuffer) {
            ringBuffer1 = RingBuffer(bufferSize);
            if (!isMono) {
                ringBuffer2 = RingBuffer(bufferSize);
            }
        } else {
            ringBuffer1 = RingBuffer();
            ringBuffer2 = RingBuffer();
        }
        setLengthNormalized(length_);

    }

void krakendsp::Delay::setSamplerate(float samplerate_) {
    samplerate = samplerate_;
    controls[2].valueMax = samplesToMS(static_cast<float>(bufferSize));
}

void krakendsp::Delay::setLengthNormalized(float newLengthNormalized) {
    delayLengthNormalized = std::clamp(newLengthNormalized, 0.0f, 1.0f);
    float newLength = delayLengthNormalized * static_cast<float>(bufferSize);
    ringBuffer1.setLengthTarget(newLength);
    if (!isMono) {
        ringBuffer2.setLengthTarget(newLength);
    }
}

void krakendsp::Delay::setLength(float newLength) {
    float clampedLength = std::clamp(newLength, 0.0f, static_cast<float>(bufferSize));
    delayLengthNormalized = clampedLength / static_cast<float>(bufferSize);
    ringBuffer1.setLengthTarget(clampedLength);
    if (!isMono) {
        ringBuffer2.setLengthTarget(clampedLength);
    }
}

void krakendsp::Delay::setLengthMS(float newLengthMS) {
    float newLength = MSToSamples(newLengthMS);
    setLength(newLength);
}

void krakendsp::Delay::setFeedback(float feedback_) {
    feedback = std::clamp(feedback_, 0.0f, 1.0f);
}

void krakendsp::Delay::setMix(float mix_) {
    mix = std::clamp(mix_, 0.0f, 1.0f);
}

float krakendsp::Delay::getSamplerate() const {
    return samplerate;
}

float krakendsp::Delay::getLengthNormalized() const {
    return delayLengthNormalized;
}

float krakendsp::Delay::getLength() const {
    return static_cast<float>(bufferSize) * delayLengthNormalized;
}

float krakendsp::Delay::getLengthMS() const {
    return samplesToMS(getLength());
}

float krakendsp::Delay::getMix() const {
    return mix;
}

float krakendsp::Delay::getFeedback() const {
    return feedback;
}

size_t krakendsp::Delay::getBufferSize() const {
    return bufferSize;
}

float krakendsp::Delay::process(float sample) {
    float delayedSample = ringBuffer1.getNextSample();
    float feedbackSample = delayedSample * feedback;
    ringBuffer1.writeNextSample(sample + feedbackSample);
    return mix * delayedSample + (1.0f - mix) * sample;
}

krakendsp::Stereo krakendsp::Delay::process(Stereo sample) {
    Stereo delayedSample = {0.0f, 0.0f};
    Stereo feedbackSample = {0.0f, 0.0f};
    Stereo output = {0.0f, 0.0f};

    delayedSample.left = ringBuffer1.getNextSample();
    feedbackSample.left = delayedSample.left * feedback;
    ringBuffer1.writeNextSample(sample.left + feedbackSample.left);
    output.left = mix * delayedSample.left + (1.0f - mix) * sample.left;

    if (isMono) {
        output.right = output.left;
    } else {
        delayedSample.right = ringBuffer2.getNextSample();
        feedbackSample.right = delayedSample.right * feedback;
        ringBuffer2.writeNextSample(sample.right + feedbackSample.right);
        output.right = mix * delayedSample.right + (1.0f - mix) * sample.right; 
    }

    return output;
}

krakendsp::FXControl krakendsp::Delay::getControl(int index) const {
    switch (index) {
        case 0: return controls[0];
        case 1: return controls[1];
        case 2: return controls[2];
        default: return krakendsp::FXControl{};
    }
}

std::string krakendsp::Delay::getName() const {
    return name;
}

void krakendsp::Delay::setControl(int index, float value) {
    switch (index) {
        case 0: setFeedback(value); break;
        case 1: setMix(value); break;
        case 2: setLengthNormalized(value); break;
        default: break;
    }
}

float krakendsp::Delay::getControlValue(int index) const {
    switch (index) {
        case 0: return getFeedback();
        case 1: return getMix();
        case 2: return getLengthNormalized();
        default: return 0;
    }
}

float krakendsp::Delay::getControlValueRaw(int index) const {
    switch (index) {
        case 0: return feedback;
        case 1: return mix;
        case 2: return delayLengthNormalized;
        default: return 0;
    }
}



std::vector<krakendsp::FXSubType> krakendsp::Delay::getTypes() const {
    return types;
}

float krakendsp::Delay::samplesToMS(float samples) const {
    return samples * 1000.0f / samplerate;
}

float krakendsp::Delay::MSToSamples(float ms) const {
    return ms * samplerate / 1000.0f;
}

krakendsp::FXSubType krakendsp::Delay::getType(int index) const {
    return types[0];
}

krakendsp::FXSubType krakendsp::Delay::getType() const {
    return types[0];
}

void krakendsp::Delay::setType(int index) {
    // Currently only one type, so no action needed{
}

std::vector<krakendsp::FXControl> krakendsp::Delay::getControls() const {
    return controls;
}

std::vector<krakendsp::Log> krakendsp::Delay::getLog() const {
    return log;
}