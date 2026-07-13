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
            fillUpSampleBuffer(sample);
            for (size_t i = 0; i < upsampleBufferLength; i++) {
                upsampleBufferL[i] = processHardClipNonAliased(upsampleBufferL[i]);
                upsampleBufferR[i] = processHardClipNonAliased(upsampleBufferR[i]);
            }
            outSample = downsampleStereo();
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
            upsampleBufferL[0] = sample;
            upsampleBufferLength = 1;
            break;
        case Oversample::x2:
            if (interpolationUpsample) {
                applyInterpolationTo2xInBuffer(sample);
            } else {
                fill2xOversampleBufferMono(sample);
                applyFilterToBufferMono(true);
            }
            break;
        case Oversample::x4:
            if (interpolationUpsample) {
                applyInterpolationTo4xInBuffer(sample);
            } else {
                fill2xOversampleBufferMono(sample);
                applyFilterToBufferMono(true);
                fill4xOversampleBufferMono();
                applyFilterToBufferMono(true);
            }
            break;
        case Oversample::x8:
            if (interpolationUpsample) {
                applyInterpolationTo8xInBuffer(sample);
            } else {
                fill2xOversampleBufferMono(sample);
                applyFilterToBufferMono(true);
                fill4xOversampleBufferMono();
                applyFilterToBufferMono(true);
                fill8xOversampleBufferMono();
                applyFilterToBufferMono(true);
            }
            break;
        default:
            break;
    }
}

void krakendsp::Distortion::fillUpSampleBuffer(Stereo sample) {
    switch (oversample) {
        case Oversample::None:
            upsampleBufferL[0] = sample.left;
            upsampleBufferR[0] = sample.right;
            upsampleBufferLength = 1;
            break;
        case Oversample::x2:
            if (interpolationUpsample) {
                applyInterpolationTo2xInBuffer(sample);
            } else {
                fill2xOversampleBufferStereo(sample);
                applyFilterToBufferStereo(true);
            }
            break;
        case Oversample::x4:
            if (interpolationUpsample) {
                applyInterpolationTo4xInBuffer(sample);
            } else {
                fill2xOversampleBufferStereo(sample);
                applyFilterToBufferStereo(true);
                fill4xOversampleBufferStereo();
                applyFilterToBufferStereo(true);
            }
            break;
        case Oversample::x8:
            if (interpolationUpsample) {
                applyInterpolationTo8xInBuffer(sample);
            } else {
                fill2xOversampleBufferStereo(sample);
                applyFilterToBufferStereo(true);
                fill4xOversampleBufferStereo();
                applyFilterToBufferStereo(true);
                fill8xOversampleBufferStereo();
                applyFilterToBufferStereo(true);
            }
            break;
        default:
            break;
    }
}
float krakendsp::Distortion::downsampleMono() {
    switch (oversample) {
        case Oversample::None:
            return upsampleBufferL[0];
        case Oversample::x2:
            applyFilterToBufferMono(false);
            return upsampleBufferL[0];
        case Oversample::x4:
            applyFilterToBufferMono(false);
            decimate2xOversampleBufferMono();
            applyFilterToBufferMono(false);
            return upsampleBufferL[0];
        case Oversample::x8:
            applyFilterToBufferMono(false);
            decimate4xOversampleBufferMono();
            applyFilterToBufferMono(false);
            decimate2xOversampleBufferMono();
            applyFilterToBufferMono(false);
            return upsampleBufferL[0];
        default:
            return upsampleBufferL[0];
    }
}

krakendsp::Stereo krakendsp::Distortion::downsampleStereo() {
    Stereo output;
    switch (oversample) {
        case Oversample::None:
            break;
        case Oversample::x2:
            applyFilterToBufferStereo(false);
            break;
        case Oversample::x4:
            applyFilterToBufferStereo(false);
            decimate2xOversampleBufferStereo();
            applyFilterToBufferStereo(false);
            break;
        case Oversample::x8:
            applyFilterToBufferStereo(false);
            decimate4xOversampleBufferStereo();
            applyFilterToBufferStereo(false);
            decimate2xOversampleBufferStereo();
            applyFilterToBufferStereo(false);
            break;
        default:
            break;
    }
    output.left = upsampleBufferL[0];
    output.right = upsampleBufferR[0];
    return output;
}

void krakendsp::Distortion::applyInterpolationTo2xInBuffer(float sample) {
    upsampleBufferL[0] = 0.5f * (previousSampleL + sample);
    upsampleBufferL[1] = sample;
    previousSampleL = sample;
    upsampleBufferLength = 2;
}

void krakendsp::Distortion::applyInterpolationTo2xInBuffer(Stereo sample) {
    upsampleBufferL[0] = 0.5f * (previousSampleL + sample.left);
    upsampleBufferL[1] = sample.left;
    previousSampleL = sample.left;
    upsampleBufferR[0] = 0.5f * (previousSampleL + sample.right);
    upsampleBufferR[1] = sample.right;
    previousSampleR = sample.right;
    upsampleBufferLength = 2;
}

void krakendsp::Distortion::applyInterpolationTo4xInBuffer(float sample) {
    upsampleBufferL[0] = 0.75f * previousSampleL + 0.25f * sample;
    upsampleBufferL[1] = 0.5f * (previousSampleL + sample);
    upsampleBufferL[2] = 0.25f * previousSampleL + 0.75f * sample;
    upsampleBufferL[3] = sample;
    previousSampleL = sample;
    upsampleBufferLength = 4;
}

void krakendsp::Distortion::applyInterpolationTo4xInBuffer(Stereo sample) {
    upsampleBufferL[0] = 0.75f * previousSampleL + 0.25f * sample.left;
    upsampleBufferL[1] = 0.5f * (previousSampleL + sample.left);
    upsampleBufferL[2] = 0.25f * previousSampleL + 0.75f * sample.left;
    upsampleBufferL[3] = sample.left;
    previousSampleL = sample.left;
    upsampleBufferR[0] = 0.75f * previousSampleL + 0.25f * sample.right;
    upsampleBufferR[1] = 0.5f * (previousSampleL + sample.right);
    upsampleBufferR[2] = 0.25f * previousSampleL + 0.75f * sample.right;
    upsampleBufferR[3] = sample.right;
    previousSampleR = sample.right;
    upsampleBufferLength = 4;
}

void krakendsp::Distortion::applyInterpolationTo8xInBuffer(float sample) {
    upsampleBufferL[0] = 0.875f * previousSampleR + 0.125f * sample;
    upsampleBufferL[1] = 0.75f * previousSampleR + 0.25f * sample;
    upsampleBufferL[2] = 0.625f * previousSampleR + 0.375f * sample;
    upsampleBufferL[3] = 0.5f * (previousSampleR + sample);
    upsampleBufferL[4] = 0.375f * previousSampleR + 0.625f * sample;
    upsampleBufferL[5] = 0.25f * previousSampleR + 0.75f * sample;
    upsampleBufferL[6] = 0.125f * previousSampleR + 0.875f * sample;
    upsampleBufferL[7] = sample;
    previousSampleL = sample;
    upsampleBufferLength = 8;
}

void krakendsp::Distortion::applyInterpolationTo8xInBuffer(Stereo sample) {
    upsampleBufferL[0] = 0.875f * previousSampleR + 0.125f * sample.left;
    upsampleBufferL[1] = 0.75f * previousSampleR + 0.25f * sample.left;
    upsampleBufferL[2] = 0.625f * previousSampleR + 0.375f * sample.left;
    upsampleBufferL[3] = 0.5f * (previousSampleR + sample.left);
    upsampleBufferL[4] = 0.375f * previousSampleR + 0.625f * sample.left;
    upsampleBufferL[5] = 0.25f * previousSampleR + 0.75f * sample.left;
    upsampleBufferL[6] = 0.125f * previousSampleR + 0.875f * sample.left;
    upsampleBufferL[7] = sample.left;
    previousSampleL = sample.left;
    upsampleBufferR[0] = 0.875f * previousSampleR + 0.125f * sample.right;
    upsampleBufferR[1] = 0.75f * previousSampleR + 0.25f * sample.right;
    upsampleBufferR[2] = 0.625f * previousSampleR + 0.375f * sample.right;
    upsampleBufferR[3] = 0.5f * (previousSampleR + sample.right);
    upsampleBufferR[4] = 0.375f * previousSampleR + 0.625f * sample.right;
    upsampleBufferR[5] = 0.25f * previousSampleR + 0.75f * sample.right;
    upsampleBufferR[6] = 0.125f * previousSampleR + 0.875f * sample.right;
    upsampleBufferR[7] = sample.right;
    previousSampleR = sample.right;
    upsampleBufferLength = 8;
}

void krakendsp::Distortion::applyFilterToBufferMono(bool inBuffer) {
    float* bufferPtr;
    size_t* bufferIndexPtr;
    if (upsampleBufferLength == 2) {
        if (inBuffer) {
            bufferPtr = aa2xInBufferL.data();
            bufferIndexPtr = &aa2xInBufferIndexL;
        } else {
            bufferPtr = aa2xOutBufferL.data();
            bufferIndexPtr = &aa2xOutBufferIndexL;
        }
    } else if (upsampleBufferLength == 4) {
        if (inBuffer) {
            bufferPtr = aa4xInBufferL.data();
            bufferIndexPtr = &aa4xInBufferIndexL;
        } else {
            bufferPtr = aa4xOutBufferL.data();
            bufferIndexPtr = &aa4xOutBufferIndexL;
        }
    } else if (upsampleBufferLength == 8) {
        if (inBuffer) {
            bufferPtr = aa8xInBufferL.data();
            bufferIndexPtr = &aa8xInBufferIndexL;
        } else {
            bufferPtr = aa8xOutBufferL.data();
            bufferIndexPtr = &aa8xOutBufferIndexL;
        }
    }

    for (size_t j = 0; j < upsampleBufferLength; j++) {
        bufferPtr[*bufferIndexPtr] = upsampleBufferL[j];
        upsampleBufferL[j] = 0.0f;
        if (inBuffer) {
            if (*bufferIndexPtr % 2 != 0) {
                for (size_t i = 0; i < halfFilterSize; i += 2) {
                    size_t tempIndex = (*bufferIndexPtr + aaFilterCoeffs.size() - i) % 256;
                    size_t tempIndex2 = (*bufferIndexPtr + i + 1) % 256;
                    upsampleBufferL[j] += (bufferPtr[tempIndex] + bufferPtr[tempIndex2]) * aaFilterCoeffs[i];
                }
            } else {
                upsampleBufferL[j] += bufferPtr[(*bufferIndexPtr - halfFilterSize + aaFilterCoeffs.size()) % 256] * aaFilterCoeffs[halfFilterSize];
            }
        } else {
            for (size_t i = 0; i < halfFilterSize; i += 2) {
                size_t tempIndex = (*bufferIndexPtr + aaFilterCoeffs.size() - i) % 256;
                size_t tempIndex2 = (*bufferIndexPtr + i + 1) % 256;
                upsampleBufferL[j] += (bufferPtr[tempIndex] + bufferPtr[tempIndex2]) * aaFilterCoeffs[i];
            }
            upsampleBufferL[j] += bufferPtr[(*bufferIndexPtr - halfFilterSize + aaFilterCoeffs.size()) % 256] * aaFilterCoeffs[halfFilterSize];
        }
        *bufferIndexPtr = (*bufferIndexPtr + 1) % 256;
    }
}

void krakendsp::Distortion::applyFilterToBufferStereo(bool inBuffer) {
    float* bufferPtrL;
    size_t* bufferIndexPtrL;
    float* bufferPtrR;
    size_t* bufferIndexPtrR;
    if (upsampleBufferLength == 2) {
        if (inBuffer) {
            bufferPtrL = aa2xInBufferL.data();
            bufferIndexPtrL = &aa2xInBufferIndexL;
            bufferPtrR = aa2xInBufferR.data();
            bufferIndexPtrR = &aa2xInBufferIndexR;
        } else {
            bufferPtrL = aa2xOutBufferL.data();
            bufferIndexPtrL = &aa2xOutBufferIndexL;
            bufferPtrR = aa2xOutBufferR.data();
            bufferIndexPtrR = &aa2xOutBufferIndexR;
        }
    } else if (upsampleBufferLength == 4) {
        if (inBuffer) {
            bufferPtrL = aa4xInBufferL.data();
            bufferIndexPtrL = &aa4xInBufferIndexL;
            bufferPtrR = aa4xInBufferR.data();
            bufferIndexPtrR = &aa4xInBufferIndexR;
        } else {
            bufferPtrL = aa4xOutBufferL.data();
            bufferIndexPtrL = &aa4xOutBufferIndexL;
            bufferPtrR = aa4xOutBufferR.data();
            bufferIndexPtrR = &aa4xOutBufferIndexR;
        }
    } else if (upsampleBufferLength == 8) {
        if (inBuffer) {
            bufferPtrL = aa8xInBufferL.data();
            bufferIndexPtrL = &aa8xInBufferIndexL;
            bufferPtrR = aa8xInBufferR.data();
            bufferIndexPtrR = &aa8xInBufferIndexR;
        } else {
            bufferPtrL = aa8xOutBufferL.data();
            bufferIndexPtrL = &aa8xOutBufferIndexL;
            bufferPtrR = aa8xOutBufferR.data();
            bufferIndexPtrR = &aa8xOutBufferIndexR;
        }
    }

    for (size_t j = 0; j < upsampleBufferLength; j++) {
        bufferPtrL[*bufferIndexPtrL] = upsampleBufferL[j];
        bufferPtrR[*bufferIndexPtrR] = upsampleBufferR[j];
        upsampleBufferL[j] = 0.0f;
        upsampleBufferR[j] = 0.0f;
        if (inBuffer) {
            if (*bufferIndexPtrL % 2 != 0) {
                for (size_t i = 0; i < halfFilterSize; i += 2) {
                    size_t tempIndex = (*bufferIndexPtrL + aaFilterCoeffs.size() - i) % 256;
                    size_t tempIndex2 = (*bufferIndexPtrL + i + 1) % 256;
                    upsampleBufferL[j] += (bufferPtrL[tempIndex] + bufferPtrL[tempIndex2]) * aaFilterCoeffs[i];
                }
            } else {
                upsampleBufferL[j] += bufferPtrL[(*bufferIndexPtrL - halfFilterSize + aaFilterCoeffs.size()) % 256] * aaFilterCoeffs[halfFilterSize];
            }
            if (*bufferIndexPtrR % 2 != 0) {
                for (size_t i = 0; i < halfFilterSize; i += 2) {
                    size_t tempIndex = (*bufferIndexPtrR + aaFilterCoeffs.size() - i) % 256;
                    size_t tempIndex2 = (*bufferIndexPtrR + i + 1) % 256;
                    upsampleBufferR[j] += (bufferPtrR[tempIndex] + bufferPtrR[tempIndex2]) * aaFilterCoeffs[i];
                }
            } else {
                upsampleBufferR[j] += bufferPtrR[(*bufferIndexPtrR - halfFilterSize + aaFilterCoeffs.size()) % 256] * aaFilterCoeffs[halfFilterSize];
            }
        } else {
            for (size_t i = 0; i < halfFilterSize; i += 2) {
                size_t tempIndexL = (*bufferIndexPtrL + aaFilterCoeffs.size() - i) % 256;
                size_t tempIndex2L = (*bufferIndexPtrL + i + 1) % 256;
                upsampleBufferL[j] += (bufferPtrL[tempIndexL] + bufferPtrL[tempIndex2L]) * aaFilterCoeffs[i];
                size_t tempIndexR = (*bufferIndexPtrR + aaFilterCoeffs.size() - i) % 256;
                size_t tempIndex2R = (*bufferIndexPtrR + i + 1) % 256;
                upsampleBufferR[j] += (bufferPtrR[tempIndexR] + bufferPtrR[tempIndex2R]) * aaFilterCoeffs[i];
            }
            upsampleBufferL[j] += bufferPtrL[(*bufferIndexPtrL - halfFilterSize + aaFilterCoeffs.size()) % 256] * aaFilterCoeffs[halfFilterSize];
            upsampleBufferR[j] += bufferPtrR[(*bufferIndexPtrR - halfFilterSize + aaFilterCoeffs.size()) % 256] * aaFilterCoeffs[halfFilterSize];
        }
        *bufferIndexPtrL = (*bufferIndexPtrL + 1) % 256;
        *bufferIndexPtrR = (*bufferIndexPtrR + 1) % 256;
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

void krakendsp::Distortion::fill2xOversampleBufferMono(float input) {
    upsampleBufferL[0] = input;
    upsampleBufferL[1] = 0.0f;
    upsampleBufferLength = 2;
}

void krakendsp::Distortion::fill2xOversampleBufferStereo(Stereo input) {
    upsampleBufferL[0] = input.left;
    upsampleBufferL[1] = 0.0f;
    upsampleBufferR[0] = input.right;
    upsampleBufferR[1] = 0.0f;
    upsampleBufferLength = 2;
}

void krakendsp::Distortion::fill4xOversampleBufferMono() {
    upsampleBufferL[0] = upsampleBufferL[0];
    upsampleBufferL[2] = upsampleBufferL[1];
    upsampleBufferL[1] = 0.0f;
    upsampleBufferL[3] = 0.0f;
    upsampleBufferLength = 4;
}

void krakendsp::Distortion::fill4xOversampleBufferStereo() {
    upsampleBufferL[0] = upsampleBufferL[0];
    upsampleBufferL[2] = upsampleBufferL[1];
    upsampleBufferL[1] = 0.0f;
    upsampleBufferL[3] = 0.0f;
    upsampleBufferR[0] = upsampleBufferR[0];
    upsampleBufferR[2] = upsampleBufferR[1];
    upsampleBufferR[1] = 0.0f;
    upsampleBufferR[3] = 0.0f;
    upsampleBufferLength = 4;
}

void krakendsp::Distortion::fill8xOversampleBufferMono() {
    upsampleBufferL[0] = upsampleBufferL[0];
    upsampleBufferL[2] = upsampleBufferL[1];
    upsampleBufferL[4] = upsampleBufferL[2];
    upsampleBufferL[6] = upsampleBufferL[3];
    upsampleBufferL[1] = 0.0f;
    upsampleBufferL[3] = 0.0f;
    upsampleBufferL[5] = 0.0f;
    upsampleBufferL[7] = 0.0f;
    upsampleBufferLength = 8;
}

void krakendsp::Distortion::fill8xOversampleBufferStereo() {
    upsampleBufferL[0] = upsampleBufferL[0];
    upsampleBufferL[2] = upsampleBufferL[1];
    upsampleBufferL[4] = upsampleBufferL[2];
    upsampleBufferL[6] = upsampleBufferL[3];
    upsampleBufferL[1] = 0.0f;
    upsampleBufferL[3] = 0.0f;
    upsampleBufferL[5] = 0.0f;
    upsampleBufferL[7] = 0.0f;
    upsampleBufferR[0] = upsampleBufferR[0];
    upsampleBufferR[2] = upsampleBufferR[1];
    upsampleBufferR[4] = upsampleBufferR[2];
    upsampleBufferR[6] = upsampleBufferR[3];
    upsampleBufferR[1] = 0.0f;
    upsampleBufferR[3] = 0.0f;
    upsampleBufferR[5] = 0.0f;
    upsampleBufferR[7] = 0.0f;
    upsampleBufferLength = 8;
}

void krakendsp::Distortion::decimate2xOversampleBufferMono() {
    upsampleBufferL[0] = upsampleBufferL[0];
    upsampleBufferL[1] = upsampleBufferL[2];
    upsampleBufferLength = 2;
}

void krakendsp::Distortion::decimate2xOversampleBufferStereo() {
    upsampleBufferL[0] = upsampleBufferL[0];
    upsampleBufferL[1] = upsampleBufferL[2];
    upsampleBufferR[0] = upsampleBufferR[0];
    upsampleBufferR[1] = upsampleBufferR[2];
    upsampleBufferLength = 2;
}

void krakendsp::Distortion::decimate4xOversampleBufferMono() {
    upsampleBufferL[0] = upsampleBufferL[0];
    upsampleBufferL[1] = upsampleBufferL[2];
    upsampleBufferL[2] = upsampleBufferL[4];
    upsampleBufferL[3] = upsampleBufferL[6];
    upsampleBufferLength = 4;
}

void krakendsp::Distortion::decimate4xOversampleBufferStereo() {
    upsampleBufferL[0] = upsampleBufferL[0];
    upsampleBufferL[1] = upsampleBufferL[2];
    upsampleBufferL[2] = upsampleBufferL[4];
    upsampleBufferL[3] = upsampleBufferL[6];
    upsampleBufferR[0] = upsampleBufferR[0];
    upsampleBufferR[1] = upsampleBufferR[2];
    upsampleBufferR[2] = upsampleBufferR[4];
    upsampleBufferR[3] = upsampleBufferR[6];
    upsampleBufferLength = 4;
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