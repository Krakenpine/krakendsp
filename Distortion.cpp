#include "Distortion.hpp"

krakendsp::Distortion::Distortion(float gain_, int type, float oversample_, float samplerate_) :
    gain(gain_),
    oversample(oversample_),
    currentType(type),
    samplerate(samplerate_) {
    
}

void krakendsp::Distortion::setSampleRate(float samplerate_) {
    samplerate = samplerate_;
}

void krakendsp::Distortion::setGain(float gain_) {
    gain = gain_;
}

void krakendsp::Distortion::setOversample(float oversample_) {
    oversample = oversample_;
}

float krakendsp::Distortion::getSamplerate() const {
    return samplerate;
}

float krakendsp::Distortion::getGain() const {
    return gain;
}

float krakendsp::Distortion::getOversample() const {
    return oversample;
}

float krakendsp::Distortion::process(float sample) {
    float outSample = 0.0f;
    switch (currentType) {
        case 0:
            outSample = processRaw(sample);
            break;
        case 1:
            outSample = processHardClip(sample);
            break;
        case 2:
            outSample = processHardClip(sample);
            break;
        case 3:
            outSample = processHardClip(sample);
            break;
        case 4:
            outSample = processWavefolder(sample);
            break;
        case 5:
            outSample = processClean(sample);
            break;
        default:
            outSample = processRaw(sample);
            break;
        }
    return outSample;
}

krakendsp::Stereo krakendsp::Distortion::process(Stereo sample) {
    Stereo outSample;
    switch (currentType) {
        case 0:
            outSample.left = processRaw(sample.left);
            outSample.right = processRaw(sample.right);
            break;
        case 1:
            outSample.left = processHardClip(sample.left);
            outSample.right = processHardClip(sample.right);
            break;
        case 2:
            outSample.left = processHardClip(sample.left);
            outSample.right = processHardClip(sample.right);
            break;
        case 3:
            outSample.left = processHardClip(sample.left);
            outSample.right = processHardClip(sample.left);
            break;
        case 4:
            outSample.left = processWavefolder(sample.left);
            outSample.right = processWavefolder(sample.right);
            break;
        case 5:
            outSample.left = processClean(sample.left);
            outSample.right = processClean(sample.right);
            break;
        default:
            outSample.left = processRaw(sample.left);
            outSample.right = processRaw(sample.right);
            break;
        }
    return outSample;
}

std::string krakendsp::Distortion::getName() const {
    return name;
}

void krakendsp::Distortion::setControl(int index, float value) {
    switch (index) {
        case 0: setGain(value); break;
        default: break;
    }
}

float krakendsp::Distortion::getControlValue(int index) const {
    switch (index) {
        case 0: return getGain();
        default: return 0;
    }
}

krakendsp::FXControl krakendsp::Distortion::getControl(int index) const {
    switch (index) {
        case 0: return controls[0];
        default: return krakendsp::FXControl{};
    }
}

float krakendsp::Distortion::getControlValueRaw(int index) const {
    switch (index) {
        case 0: return gain;
        default: return 0.0f;
    }
}

std::vector<krakendsp::FXControl> krakendsp::Distortion::getControls() const {
    return controls;
}

std::vector<krakendsp::FXSubType> krakendsp::Distortion::getTypes() const {
    return types;
}

void krakendsp::Distortion::setType(int index) {
    if (index >= 0 && index < static_cast<int>(types.size())) {
        currentType = index;
    }
}

krakendsp::FXSubType krakendsp::Distortion::getType(int index) const {
    return types[index];
}

krakendsp::FXSubType krakendsp::Distortion::getType() const {
    return types[currentType];
}

float krakendsp::Distortion::processRaw(float input) {
    return input;
}

float krakendsp::Distortion::processClean(float input) {
    float sample = input * gain * 5.0f;
    if (sample < -3.f) { sample = -1.f; }
    else if (sample > 3.f) { sample = 1.f; }
    else { sample = sample * (27.f + sample*sample) / (27.f + sample * sample * 9.f); }
    return sample;
}

float krakendsp::Distortion::processHardClip(float input) {
    float sample = input * gain * gain * gain * 1000.0f;


    if (sample > 1) {
        sample = 1;
    } else if (sample < -1) {
        sample = -1;
    }
    return sample;
}

float krakendsp::Distortion::processWavefolder(float input) {
    float polarity = 1;
    if (input < 0) {
        polarity = -1;
    }

    float sample = fabs(input);

    sample = input * gain * 10.0f;

    for (int i = 0; i < 10; i ++) {
        if (i % 2 == 0) {
            if (sample < 0) {
                sample = -1.0f * sample;
            }
        } else {
            if (sample > 1) {
                sample = 1 - 1.0f * (sample - 1);
            }
        }
    }

    if (sample > 1) {
        sample = 1;
    }

    sample *= polarity;

    return sample;
}

std::vector<krakendsp::Log> krakendsp::Distortion::getLog() const {
    return log;
}