#include "Distortion.hpp"

krakendsp::Distortion::Distortion(float gain_, int type, size_t oversample_, float samplerate_) :
    gain(gain_),
    currentType(type),
    samplerate(samplerate_) {
        setOversample(oversample_);
    }

void krakendsp::Distortion::setSampleRate(float samplerate_) {
    samplerate = samplerate_;
}

void krakendsp::Distortion::setGain(float gain_) {
    gain = gain_;
}

void krakendsp::Distortion::setOversample(size_t oversample_) {
    if (oversample_ < 1 || oversample_ == 1) {
        oversample = Oversample::None;
    } else if (oversample_ == 2 || oversample_ == 3) {
        oversample = Oversample::x2;
    } else if (oversample_ == 4 || oversample_ == 5 || oversample_ == 6 || oversample_ == 7) {
        oversample = Oversample::x4;
    } else {
        oversample = Oversample::x8;
    }
 }

float krakendsp::Distortion::getSamplerate() const {
    return samplerate;
}

float krakendsp::Distortion::getGain() const {
    return gain;
}

size_t krakendsp::Distortion::getOversample() const {
    switch (oversample) {
        case Oversample::None: return 1;
        case Oversample::x2: return 2;
        case Oversample::x4: return 4;
        case Oversample::x8: return 8;
        default: return 1;
    }
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
            outSample.right = outSample.left;
            break;
        case 1:
            outSample.left = processSoftClip(sample.left);
            outSample.right = outSample.left;
            break;
        case 2:
            fillUpSampleBuffer(sample.left);
            for (size_t i = 0; i < upsampleBufferLength; i++) {
                upsampleBuffer[i] = processHardClipNonAliased(upsampleBuffer[i]);
            }
            outSample.left = downsample();
            outSample.right = outSample.left;
            break;
        case 3:
            outSample.left = processHardClip(sample.left);
            outSample.right = outSample.left;
            break;
        case 4:
            outSample.left = processWavefolder(sample.left);
            outSample.right = outSample.left;
            break;
        case 5:
            outSample.left = processClean(sample.left);
            outSample.right = outSample.left;
            break;
        default:
            outSample.left = processRaw(sample.left);
            outSample.right = outSample.left;
            break;
        }
    return outSample;
}

void krakendsp::Distortion::fillUpSampleBuffer(float sample) {
    switch (oversample) {
        case Oversample::None:
            upsampleBuffer[0] = sample;
            upsampleBufferLength = 1;
            break;
        case Oversample::x2:
            if (interpolationUpsample) {
                applyInterpolationTo2xInBuffer(sample);
            } else {
                fill2xOversampleBuffer(sample);
                applyFilterTo2xInBuffer();
            }
            break;
        case Oversample::x4:
            if (interpolationUpsample) {
                applyInterpolationTo4xInBuffer(sample);
            } else {
                fill2xOversampleBuffer(sample);
                applyFilterTo2xInBuffer();
                fill4xOversampleBuffer();
                applyFilterTo4xInBuffer();
            }
            break;
        case Oversample::x8:
            if (interpolationUpsample) {
                applyInterpolationTo8xInBuffer(sample);
            } else {
                fill2xOversampleBuffer(sample);
                applyFilterTo2xInBuffer();
                fill4xOversampleBuffer();
                applyFilterTo4xInBuffer();
                fill8xOversampleBuffer();
                applyFilterTo8xInBuffer();
            }
            break;
        default:
            break;
    }
}

float krakendsp::Distortion::downsample() {
    switch (oversample) {
        case Oversample::None:
            return upsampleBuffer[0];
        case Oversample::x2:
            applyFilterTo2xOutBuffer();
            return upsampleBuffer[0];
        case Oversample::x4:
            applyFilterTo4xOutBuffer();
            decimate2xOversampleBuffer();
            applyFilterTo2xOutBuffer();
            return upsampleBuffer[0];
        case Oversample::x8:
            applyFilterTo8xOutBuffer();
            decimate4xOversampleBuffer();
            applyFilterTo4xOutBuffer();
            decimate2xOversampleBuffer();
            applyFilterTo2xOutBuffer();
            return upsampleBuffer[0];
        default:
            return upsampleBuffer[0];
    }
}



void krakendsp::Distortion::applyInterpolationTo2xInBuffer(float sample) {
    upsampleBuffer[0] = 0.5f * (previousSample + sample);
    upsampleBuffer[1] = sample;
    previousSample = sample;
    upsampleBufferLength = 2;
}

void krakendsp::Distortion::applyInterpolationTo4xInBuffer(float sample) {
    upsampleBuffer[0] = 0.75f * previousSample + 0.25f * sample;
    upsampleBuffer[1] = 0.5f * (previousSample + sample);
    upsampleBuffer[2] = 0.25f * previousSample + 0.75f * sample;
    upsampleBuffer[3] = sample;
    previousSample = sample;
    upsampleBufferLength = 4;
}

void krakendsp::Distortion::applyInterpolationTo8xInBuffer(float sample) {
    upsampleBuffer[0] = 0.875f * previousSample + 0.125f * sample;
    upsampleBuffer[1] = 0.75f * previousSample + 0.25f * sample;
    upsampleBuffer[2] = 0.625f * previousSample + 0.375f * sample;
    upsampleBuffer[3] = 0.5f * (previousSample + sample);
    upsampleBuffer[4] = 0.375f * previousSample + 0.625f * sample;
    upsampleBuffer[5] = 0.25f * previousSample + 0.75f * sample;
    upsampleBuffer[6] = 0.125f * previousSample + 0.875f * sample;
    upsampleBuffer[7] = sample;
    previousSample = sample;
    upsampleBufferLength = 8;
}

void krakendsp::Distortion::applyFilterTo2xInBuffer() {
    for (size_t j = 0; j < 2; j++) {
        aa2xInBuffer[aa2xInBufferIndex] = upsampleBuffer[j];
        upsampleBuffer[j] = 0.0f;
        if (aa2xInBufferIndex % 2 != 0) {
            for (size_t i = 0; i < halfFilterSize; i += 2) {
                size_t tempIndex = (aa2xInBufferIndex + aaFilterCoeffs.size() - i) % 256;
                size_t tempIndex2 = (aa2xInBufferIndex + i + 1) % 256;
                upsampleBuffer[j] += (aa2xInBuffer[tempIndex] + aa2xInBuffer[tempIndex2]) * aaFilterCoeffs[i];
            }
        } else {
            upsampleBuffer[j] += aa2xInBuffer[(aa2xInBufferIndex - halfFilterSize + aaFilterCoeffs.size()) % 256] * aaFilterCoeffs[halfFilterSize];
        }
        aa2xInBufferIndex = (aa2xInBufferIndex + 1) % 256;
    }
}


void krakendsp::Distortion::applyFilterTo4xInBuffer() {
    for (size_t j = 0; j < 4; j++) {
        aa4xInBuffer[aa4xInBufferIndex] = upsampleBuffer[j];
        upsampleBuffer[j] = 0.0f;
        if (aa4xInBufferIndex % 2 != 0) {
            for (size_t i = 0; i < halfFilterSize; i += 2) {
                size_t tempIndex = (aa4xInBufferIndex + aaFilterCoeffs.size() - i) % 256;
                size_t tempIndex2 = (aa4xInBufferIndex + i + 1) % 256;
                upsampleBuffer[j] += (aa4xInBuffer[tempIndex] + aa4xInBuffer[tempIndex2]) * aaFilterCoeffs[i];
            }
        } else {
            upsampleBuffer[j] += aa4xInBuffer[(aa4xInBufferIndex - halfFilterSize + aaFilterCoeffs.size()) % 256] * aaFilterCoeffs[halfFilterSize];
        }
        aa4xInBufferIndex = (aa4xInBufferIndex + 1) % 256;
    } 
}

void krakendsp::Distortion::applyFilterTo8xInBuffer() {
    for (size_t j = 0; j < 8; j++) {
        aa8xInBuffer[aa8xInBufferIndex] = upsampleBuffer[j];
        upsampleBuffer[j] = 0.0f;
        if (aa8xInBufferIndex % 2 != 0) {
            for (size_t i = 0; i < halfFilterSize; i += 2) {
                size_t tempIndex = (aa8xInBufferIndex + aaFilterCoeffs.size() - i) % 256;
                size_t tempIndex2 = (aa8xInBufferIndex + i + 1) % 256;
                upsampleBuffer[j] += (aa8xInBuffer[tempIndex] + aa8xInBuffer[tempIndex2]) * aaFilterCoeffs[i];
            }
        } else {
            upsampleBuffer[j] += aa8xInBuffer[(aa8xInBufferIndex - halfFilterSize + aaFilterCoeffs.size()) % 256] * aaFilterCoeffs[halfFilterSize];
        }   
        aa8xInBufferIndex = (aa8xInBufferIndex + 1) % 256;
    }
}

void krakendsp::Distortion::applyFilterTo2xOutBuffer() {
    for (size_t j = 0; j < 2; j++) {
        aa2xOutBuffer[aa2xOutBufferIndex] = upsampleBuffer[j];
        upsampleBuffer[j] = 0.0f;
        for (size_t i = 0; i < halfFilterSize; i += 2) {
            size_t tempIndex = (aa2xOutBufferIndex + aaFilterCoeffs.size() - i) % 256;
            size_t tempIndex2 = (aa2xOutBufferIndex + i + 1) % 256;
            upsampleBuffer[j] += (aa2xOutBuffer[tempIndex] + aa2xOutBuffer[tempIndex2]) * aaFilterCoeffs[i];
        }
        upsampleBuffer[j] += aa2xOutBuffer[(aa2xOutBufferIndex - halfFilterSize + aaFilterCoeffs.size()) % 256] * aaFilterCoeffs[halfFilterSize];
        aa2xOutBufferIndex = (aa2xOutBufferIndex + 1) % 256;
    }
    
}

void krakendsp::Distortion::applyFilterTo4xOutBuffer() {
    for (size_t j = 0; j < 4; j++) {
        aa4xOutBuffer[aa4xOutBufferIndex] = upsampleBuffer[j];
        upsampleBuffer[j] = 0.0f;
        for (size_t i = 0; i < halfFilterSize; i += 2) {
            size_t tempIndex = (aa4xOutBufferIndex + aaFilterCoeffs.size() - i) % 256;
            size_t tempIndex2 = (aa4xOutBufferIndex + i + 1) % 256;
            upsampleBuffer[j] += (aa4xOutBuffer[tempIndex] + aa4xOutBuffer[tempIndex2]) * aaFilterCoeffs[i];
        }
        upsampleBuffer[j] += aa4xOutBuffer[(aa4xOutBufferIndex - halfFilterSize + aaFilterCoeffs.size()) % 256] * aaFilterCoeffs[halfFilterSize];
        aa4xOutBufferIndex = (aa4xOutBufferIndex + 1) % 256;
    }
}

void krakendsp::Distortion::applyFilterTo8xOutBuffer() {
    for (size_t j = 0; j < 8; j++) {
        aa8xOutBuffer[aa8xOutBufferIndex] = upsampleBuffer[j];
        upsampleBuffer[j] = 0.0f;
        for (size_t i = 0; i < halfFilterSize; i += 2) {
            size_t tempIndex = (aa8xOutBufferIndex + aaFilterCoeffs.size() - i) % 256;
            size_t tempIndex2 = (aa8xOutBufferIndex + i + 1) % 256;
            upsampleBuffer[j] += (aa8xOutBuffer[tempIndex] + aa8xOutBuffer[tempIndex2]) * aaFilterCoeffs[i];
        }
        upsampleBuffer[j] += aa8xOutBuffer[(aa8xOutBufferIndex - halfFilterSize + aaFilterCoeffs.size()) % 256] * aaFilterCoeffs[halfFilterSize];
        aa8xOutBufferIndex = (aa8xOutBufferIndex + 1) % 256;
    }
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
    float realGain = gain * gain * gain * 1000.0f;
    float sample = input * realGain;
    
    sample = sample * realGain;
    if (sample > 1) {
        sample = 1;
    } else if (sample < -1) {
        sample = -1;
    }
    //if (sample < -3.f) { sample = -1.f; }
    //else if (sample > 3.f) { sample = 1.f; }
    //else { sample = sample * (27.f + sample*sample) / (27.f + sample * sample * 9.f); }

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

void krakendsp::Distortion::fill2xOversampleBuffer(float input) {
    upsampleBuffer[0] = input;
    upsampleBuffer[1] = 0.0f;
    upsampleBufferLength = 2;
}

void krakendsp::Distortion::fill4xOversampleBuffer() {
    upsampleBuffer[0] = upsampleBuffer[0];
    upsampleBuffer[2] = upsampleBuffer[1];
    upsampleBuffer[1] = 0.0f;
    upsampleBuffer[3] = 0.0f;
    upsampleBufferLength = 4;
}

void krakendsp::Distortion::fill8xOversampleBuffer() {
    upsampleBuffer[0] = upsampleBuffer[0];
    upsampleBuffer[2] = upsampleBuffer[1];
    upsampleBuffer[4] = upsampleBuffer[2];
    upsampleBuffer[6] = upsampleBuffer[3];
    upsampleBuffer[1] = 0.0f;
    upsampleBuffer[3] = 0.0f;
    upsampleBuffer[5] = 0.0f;
    upsampleBuffer[7] = 0.0f;
    upsampleBufferLength = 8;
}

void krakendsp::Distortion::decimate2xOversampleBuffer() {
    upsampleBuffer[0] = upsampleBuffer[0];
    upsampleBuffer[1] = upsampleBuffer[2];
}

void krakendsp::Distortion::decimate4xOversampleBuffer() {
    upsampleBuffer[0] = upsampleBuffer[0];
    upsampleBuffer[1] = upsampleBuffer[2];
    upsampleBuffer[2] = upsampleBuffer[4];
    upsampleBuffer[3] = upsampleBuffer[6];
}

void krakendsp::Distortion::setInterpolationUpsampleMode(bool _interpolationUpsample) {
    interpolationUpsample = _interpolationUpsample;
}

bool krakendsp::Distortion::getInterpolationUpsampleMode() {
    return interpolationUpsample;
}

std::vector<krakendsp::Log> krakendsp::Distortion::getLog() const {
    return log;
}