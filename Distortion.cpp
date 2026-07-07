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
            outSample = processHardClipNonAliased(sample);
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
            outSample.left = processSoftClip(sample.left);
            outSample.right = processSoftClip(sample.right);
            break;
        case 2:
            outSample.left = processHardClipNonAliased(sample.left);
            outSample.right = processHardClipNonAliased(sample.right);
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

float krakendsp::Distortion::processSoftClip(float input) {
    float sample = input * gain * 10.0f;
    if (sample < -3.f) { sample = -1.f; }
    else if (sample > 3.f) { sample = 1.f; }
    else { sample = sample * (27.f + sample*sample) / (27.f + sample * sample * 9.f); }

    sample = sample * gain * 10.0f;
    
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

float krakendsp::Distortion::processHardClipNonAliased(float input) {
    float sample = input * gain * gain * gain * 1000.0f;

    float sample0 = 0.75f * prevValue + 0.25f * input;
    float sample1 = 0.50f * prevValue + 0.50f * input;
    float sample2 = 0.25f * prevValue + 0.75f * input;
    float sample3 = input;

    float realGain = gain * gain * gain * 1000.0f;

    sample0 *= realGain;
    sample1 *= realGain;
    sample2 *= realGain;
    sample3 *= realGain;


    if (sample0 < -3.f) { sample0 = -1.f; }
    else if (sample0 > 3.f) { sample0 = 1.f; }
    else { sample0 = sample0 * (27.f + sample0*sample0) / (27.f + sample0 * sample0 * 9.f); }

    if (sample1 < -3.f) { sample1 = -1.f; }
    else if (sample1 > 3.f) { sample1 = 1.f; }
    else { sample1 = sample1 * (27.f + sample1*sample1) / (27.f + sample1 * sample1 * 9.f); }

    if (sample2 < -3.f) { sample2 = -1.f; }
    else if (sample2 > 3.f) { sample2 = 1.f; }
    else { sample2 = sample2 * (27.f + sample2*sample2) / (27.f + sample2 * sample2 * 9.f); }

    if (sample3 < -3.f) { sample3 = -1.f; }
    else if (sample3 > 3.f) { sample3 = 1.f; }
    else { sample3 = sample3 * (27.f + sample3*sample3) / (27.f + sample3 * sample3 * 9.f); }

    prevValue = input;

    float alpha = 3.14f * 18000.0f * (1.0f / (4.0f * samplerate));
    float beta = 1.0f - alpha;

    float output0 = ((beta)/(1+alpha))*prevOutput + ((alpha)/(1+alpha))*(sample0 + prevFiltInput);
    float output1 = ((beta)/(1+alpha))*output0 + ((alpha)/(1+alpha))*(sample1 + sample0);
    float output2 = ((beta)/(1+alpha))*output1 + ((alpha)/(1+alpha))*(sample2 + sample1);
    float output3 = ((beta)/(1+alpha))*output2 + ((alpha)/(1+alpha))*(sample3 + sample2);

    float output = 0.25f * output0 + 0.25f * output1 + 0.25f * output2 + 0.25f * output3;

    prevFiltInput = sample3;
    prevOutput = output;

    return output;
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