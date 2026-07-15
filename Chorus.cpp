#include "Chorus.hpp"
#include <algorithm>

krakendsp::Chorus::Chorus(float rate_, float depth_, float minDepth_, float mix_, float feedback_, float samplerate_, size_t bufferSize_, float* buffer1_, float* buffer2_, bool isMono_) :
    rate(rate_),
    depth(depth_),
    minDepth(minDepth_),
    mix(mix_),
    feedback(feedback_),
    samplerate(samplerate_),
    isMono(isMono_)
    {
        lfo = LFO(rate_, samplerate_);
        lfo.setType(krakendsp::LFOType::SINE);
        delay1 = Delay(0.0f, mix_, minDepth_ + depth_, samplerate_, 44100, true, true, nullptr, nullptr);
    }

float krakendsp::Chorus::process(float sample) {
    float lfoValue = lfo.getNextSampleUnipolar();
    float delayLengthInSamples = scaleValue(delayMinLengthSamples, delayMaxLengthSamples, lfoValue);
    delay1.setLength(delayLengthInSamples);
    return delay1.process(sample);
}

krakendsp::Stereo krakendsp::Chorus::process(Stereo sample) {
    Stereo output = {0.0f, 0.0f};
    float lfoValue = lfo.getNextSampleUnipolar();
    float delayLengthInSamples = scaleValue(delayMinLengthSamples, delayMaxLengthSamples, lfoValue);
    delay1.setLength(delayLengthInSamples);
    output.left = delay1.process(sample.left);
    output.right = output.left;
    return output;
}

void krakendsp::Chorus::setSamplerate(float samplerate_) {
    samplerate = samplerate_;
    lfo.setSampleRate(samplerate_);
    delay1.setSamplerate(samplerate_);
}

void krakendsp::Chorus::setRate(float rate_) {
    rateNormalized = std::clamp(rate_ * rate_, 0.0f, 1.0f);
    rate = scaleValue(controls[0].valueMin, controls[0].valueMax, rateNormalized);
    lfo.setFrequency(rate);
}

void krakendsp::Chorus::setDepth(float depth_) {
    depthNormalized = std::clamp(depth_, 0.0f, 1.0f);
    depth = scaleValue(controls[1].valueMin, controls[1].valueMax, depthNormalized);
    delay1.setLengthMS(minDepth + depth);
    delayMaxLengthSamples = MSToSamples(depth, samplerate);
}

void krakendsp::Chorus::setMinDepth(float minDepth_) {
    minDepthNormalized = std::clamp(minDepth_, 0.0f, 1.0f);
    minDepth = scaleValue(controls[2].valueMin, controls[2].valueMax, minDepthNormalized);
    delayMinLengthSamples = MSToSamples(minDepth, samplerate);
}

void krakendsp::Chorus::setMix(float mix_) {
    mix = std::clamp(mix_, 0.0f, 1.0f);
    delay1.setMix(mix);
}

void krakendsp::Chorus::setFeedback(float feedback_) {
    feedback = std::clamp(feedback_, 0.0f, 1.0f);
    delay1.setFeedback(feedback);
}

float krakendsp::Chorus::getSamplerate() const {
    return samplerate;
}

float krakendsp::Chorus::getRate() const {
    return rateNormalized;
}

float krakendsp::Chorus::getRateNonNormalized() const {
    return rate;
}

float krakendsp::Chorus::getDepth() const {
    return depthNormalized;
}

float krakendsp::Chorus::getDepthNonNormalized() const {
    return depth;
}

float krakendsp::Chorus::getMinDepth() const {
    return minDepthNormalized;
}

float krakendsp::Chorus::getMinDepthNonNormalized() const {
    return minDepth;
}

float krakendsp::Chorus::getMix() const {
    return mix;
}

float krakendsp::Chorus::getFeedback() const {
    return feedback;
}

float krakendsp::Chorus::getControlValue(int index) const {
    switch (index) {
        case 0: return getRateNonNormalized();
        case 1: return getDepthNonNormalized();
        case 2: return getMinDepthNonNormalized();
        case 3: return getMix();
        case 4: return getFeedback();
        default: return 0.0f;
    }
}

float krakendsp::Chorus::getControlValueRaw(int index) const {
    switch (index) {
        case 0: return getRate();
        case 1: return getDepth();
        case 2: return getMinDepth();
        case 3: return getMix();
        case 4: return getFeedback();
        default: return 0.0f;
    }
}

std::vector<krakendsp::FXControl> krakendsp::Chorus::getControls() const {
    return controls;
}

krakendsp::FXControl krakendsp::Chorus::getControl(int index) const {
    switch (index) {
        case 0: return controls[0];
        case 1: return controls[1];
        case 2: return controls[2];
        case 3: return controls[3];
        case 4: return controls[4];
        default: return krakendsp::FXControl{};
    }
}

void krakendsp::Chorus::setControl(int index, float value) {
    switch (index) {
        case 0: setRate(value); break;
        case 1: setDepth(value); break;
        case 2: setMinDepth(value); break;
        case 3: setMix(value); break;
        case 4: setFeedback(value); break;
        default: break;
    }
}

std::vector<krakendsp::FXSubType> krakendsp::Chorus::getTypes() const {
    return types;
}

krakendsp::FXSubType krakendsp::Chorus::getType(int index) const {
    switch (index) {
        case 0: return types[0];
        default: return krakendsp::FXSubType{};
    }
}

krakendsp::FXSubType krakendsp::Chorus::getType() const {
    return types[type];
}

void krakendsp::Chorus::setType(int index) {
    switch (index) {
        case 0: break; // only one type at the moment
        default: break;
    }
}

std::string krakendsp::Chorus::getName() const {
    return name;
}

std::vector<krakendsp::Log> krakendsp::Chorus::getLog() const {
    return log;
}