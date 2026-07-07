#include "Tremolo.hpp"
#include "LFO.hpp"
#include <algorithm>

krakendsp::Tremolo::Tremolo(float depth_, float rate_, float sampleRate_) :
    depth(depth_),
    rate(rate_),
    sampleRate(sampleRate_)
{
    rateNormalized = rate;
    lfo.setFrequency(rate);
    lfo.setSampleRate(sampleRate);
}

float krakendsp::Tremolo::process(float input) {
    float lfoSample = lfo.getNextSampleUnipolar(); // Get unipolar LFO sample (0.0 to 1.0)
    return input * (1.0f - depth) + input * depth * lfoSample;
}

krakendsp::Stereo krakendsp::Tremolo::process(Stereo input) {
    float lfoSample = lfo.getNextSampleUnipolar();
    Stereo output;
    output.left = input.left * (1.0f - depth) + input.left * depth * lfoSample;
    output.right = input.right * (1.0f - depth) + input.right * depth * lfoSample;
    return output;
}

void krakendsp::Tremolo::setDepth(float depth_) {
    depth = std::clamp(depth_, 0.0f, 1.0f);
}

void krakendsp::Tremolo::setRate(float rate_) {
    rateNormalized = rate_;
    rate = rate_ * 10.0f;
    lfo.setFrequency(rate);
}

void krakendsp::Tremolo::setSampleRate(float sampleRate_) {
    sampleRate = sampleRate_;
    lfo.setSampleRate(sampleRate);
}

void krakendsp::Tremolo::setLFOType(LFOType type_) {
    lfo.setType(type_);
}

krakendsp::FXControl krakendsp::Tremolo::getControl(int index) const {
    switch (index) {
        case 0: return controls[0];
        case 1: return controls[1];
        case 2: return controls[2];
        case 3: return controls[3];
        case 4: return controls[4];
        default: return krakendsp::FXControl{};
            //throw std::out_of_range("Index out of range for FXControl array.");
    }
}

std::string krakendsp::Tremolo::getName() const {
    return name;
}

std::vector<krakendsp::FXControl> krakendsp::Tremolo::getControls() const {
    return controls;
}

void krakendsp::Tremolo::setControl(int index, float value) {
    switch (index) {
        case 0: setDepth(value); break;
        case 1: setRate(value); break;
        case 2: lfo.setHorizontalDistort(value); break;
        case 3: lfo.setVerticalDistort(value); break;
        case 4: lfo.setFilter(value); break;
        default: break;
            //throw std::out_of_range("Index out of range for FXControl array.");
    }
}

float krakendsp::Tremolo::getControlValue(int index) const {
    switch (index) {
        case 0: return depth;
        case 1: return rateNormalized;
        case 2: return lfo.getHorizontalDistort();
        case 3: return lfo.getVerticalDistort();
        case 4: return lfo.getFilterFreq() / 100.0f;
        default:
            return 0.0f;
            //throw std::out_of_range("Index out of range for FXControl array.");
    }
}

float krakendsp::Tremolo::getControlValueRaw(int index) const {
    switch (index) {
        case 0: return depth;
        case 1: return rateNormalized;
        case 2: return lfo.getHorizontalDistort();
        case 3: return lfo.getVerticalDistort();
        case 4: return lfo.getFilterFreq();
        default:
            return 0.0f;
            //throw std::out_of_range("Index out of range for FXControl array.");
    }
}

std::vector<krakendsp::FXSubType> krakendsp::Tremolo::getTypes() const {
    return types;
}

krakendsp::FXSubType krakendsp::Tremolo::getType(int index) const {
    return types[index];
}

krakendsp::FXSubType krakendsp::Tremolo::getType() const {
    switch (lfo.getType()) {
        case LFOType::SINE: return types[0];
        case LFOType::SQUARE: return types[1];
        case LFOType::TRIANGLE: return types[2];
        case LFOType::SAWTOOTH: return types[3];
        default: return FXSubType{};
    }
}

void krakendsp::Tremolo::setType(int index) {
    lfo.setType(static_cast<LFOType>(index));
}

std::vector<krakendsp::Log> krakendsp::Tremolo::getLog() const {
    return log;
}