#include "Distortion.hpp"

krakendsp::Distortion::Distortion(float gain_, int type, size_t oversample_, float samplerate_) :
    gain(gain_),
    currentType(type),
    samplerate(samplerate_) {
        setOversample(oversample_);
        filter1.setSamplerate(samplerate);
    }

void krakendsp::Distortion::setSampleRate(float samplerate_) {
    samplerate = samplerate_;
    filter1.setSamplerate(samplerate);
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
    fillUpSampleBuffer(sample);
    for (size_t i = 0; i < upsampleBufferLength; i++) {
        switch (currentType) {
            case 0:
                upsampleBufferVector[i].left = processRaw(upsampleBufferVector[i].left);
                break;
            case 1:
                upsampleBufferVector[i].left = processSoftClip(upsampleBufferVector[i].left);
                break;
            case 2:
                upsampleBufferVector[i].left = processHardClip(upsampleBufferVector[i].left);
                break;
            case 3:
                upsampleBufferVector[i].left = processHardClip(upsampleBufferVector[i].left);
                break;
            case 4:
                upsampleBufferVector[i].left = processWavefolder(upsampleBufferVector[i].left);
                break;
            case 5:
                upsampleBufferVector[i].left = processClean(upsampleBufferVector[i].left);
                break;
            default:
                upsampleBufferVector[i].left = processRaw(upsampleBufferVector[i].left);
                break;
        }
    }
    outSample = downsampleStereo().left;
    return outSample;
}

krakendsp::Stereo krakendsp::Distortion::process(Stereo sample) {
    Stereo outSample;
    fillUpSampleBuffer(sample);
    for (size_t i = 0; i < upsampleBufferLength; i++) {
        switch (currentType) {
            case 0:
                upsampleBufferVector[i].left = processRaw(upsampleBufferVector[i].left);
                upsampleBufferVector[i].right = processRaw(upsampleBufferVector[i].right);
                break;
            case 1:
                upsampleBufferVector[i].left = processSoftClip(upsampleBufferVector[i].left);
                upsampleBufferVector[i].right = processSoftClip(upsampleBufferVector[i].right);
                break;
            case 2:
                upsampleBufferVector[i].left = processHardClip(upsampleBufferVector[i].left);
                upsampleBufferVector[i].right = processHardClip(upsampleBufferVector[i].right);
                break;
            case 3:
                upsampleBufferVector[i].left = processHardClip(upsampleBufferVector[i].left);
                upsampleBufferVector[i].right = processHardClip(upsampleBufferVector[i].right);
                break;
            case 4:
                upsampleBufferVector[i].left = processWavefolder(upsampleBufferVector[i].left);
                upsampleBufferVector[i].right = processWavefolder(upsampleBufferVector[i].right);
                break;
            case 5:
                upsampleBufferVector[i].left = processClean(upsampleBufferVector[i].left);
                upsampleBufferVector[i].right = processClean(upsampleBufferVector[i].right);
                break;
            default:
                upsampleBufferVector[i].left = processRaw(upsampleBufferVector[i].left);
                upsampleBufferVector[i].right = processRaw(upsampleBufferVector[i].right);
                break;
        }
    }
    outSample = downsampleStereo();
    //outSample = filter1.process(outSample);
    return outSample;
}

void krakendsp::Distortion::fillUpSampleBuffer(float sample) {
    krakendsp::Stereo sSample = {sample, 0.0f};
    switch (oversample) {
        case Oversample::None:
            upsampleBufferVector[0] = sSample;
            upsampleBufferLength = 1;
            break;
        case Oversample::x2:
            if (interpolationUpsample) {
                applyInterpolationTo2xInBuffer(sSample);
            } else {
                fill2xOversampleBufferStereo(sSample);
                applyFilterToBufferStereoWithVectors(true, true);
            }
            break;
        case Oversample::x4:
            if (interpolationUpsample) {
                applyInterpolationTo4xInBuffer(sSample);
            } else {
                fill2xOversampleBufferStereo(sSample);
                applyFilterToBufferStereoWithVectors(true, true);
                fill4xOversampleBufferStereo();
                applyFilterToBufferStereoWithVectors(true, true);
            }
            break;
        case Oversample::x8:
            if (interpolationUpsample) {
                applyInterpolationTo8xInBuffer(sSample);
            } else {
                fill2xOversampleBufferStereo(sSample);
                applyFilterToBufferStereoWithVectors(true, true);
                fill4xOversampleBufferStereo();
                applyFilterToBufferStereoWithVectors(true, true);
                fill8xOversampleBufferStereo();
                applyFilterToBufferStereoWithVectors(true, true);
            }
            break;
        default:
            break;
    }
}

void krakendsp::Distortion::fillUpSampleBuffer(Stereo sample) {
    switch (oversample) {
        case Oversample::None:
            upsampleBufferLength = 1;
            upsampleBufferVector[0] = sample;
            break;
        case Oversample::x2:
            if (interpolationUpsample) {
                applyInterpolationTo2xInBuffer(sample);
            } else {
                fill2xOversampleBufferStereo(sample);
                applyFilterToBufferStereoWithVectors(true, false);
            }
            break;
        case Oversample::x4:
            if (interpolationUpsample) {
                applyInterpolationTo4xInBuffer(sample);
            } else {
                fill2xOversampleBufferStereo(sample);
                applyFilterToBufferStereoWithVectors(true, false);
                fill4xOversampleBufferStereo();
                applyFilterToBufferStereoWithVectors(true, false);
            }
            break;
        case Oversample::x8:
            if (interpolationUpsample) {
                applyInterpolationTo8xInBuffer(sample);
            } else {
                fill2xOversampleBufferStereo(sample);
                applyFilterToBufferStereoWithVectors(true, false);
                fill4xOversampleBufferStereo();
                applyFilterToBufferStereoWithVectors(true, false);
                fill8xOversampleBufferStereo();
                applyFilterToBufferStereoWithVectors(true, false);
            }
            break;
        default:
            break;
    }
}

krakendsp::Stereo krakendsp::Distortion::downsampleStereo() {
    Stereo output;
    switch (oversample) {
        case Oversample::None:
            break;
        case Oversample::x2:
            applyFilterToBufferStereoWithVectors(false, false);
            break;
        case Oversample::x4:
            applyFilterToBufferStereoWithVectors(false, false);
            decimate2xOversampleBufferStereo();
            applyFilterToBufferStereoWithVectors(false, false);
            break;
        case Oversample::x8:
            applyFilterToBufferStereoWithVectors(false, false);
            decimate4xOversampleBufferStereo();
            applyFilterToBufferStereoWithVectors(false, false);
            decimate2xOversampleBufferStereo();
            applyFilterToBufferStereoWithVectors(false, false);
            break;
        default:
            break;
    }
    output = upsampleBufferVector[0];
    return output;
}

void krakendsp::Distortion::applyInterpolationTo2xInBuffer(Stereo sample) {
    upsampleBufferVector[0] = 0.5f * (previousSample + sample);
    upsampleBufferVector[1] = sample;
    previousSample = sample;
    
    upsampleBufferLength = 2;
}

void krakendsp::Distortion::applyInterpolationTo4xInBuffer(Stereo sample) {
    upsampleBufferVector[0] = 0.75f * previousSample + 0.25f * sample;
    upsampleBufferVector[1] = 0.5f * (previousSample + sample);
    upsampleBufferVector[2] = 0.25f * previousSample + 0.75f * sample;
    upsampleBufferVector[3] = sample;
    previousSample = sample;
    
    upsampleBufferLength = 4;
}

void krakendsp::Distortion::applyInterpolationTo8xInBuffer(Stereo sample) {
    upsampleBufferVector[0] = 0.875f * previousSample + 0.125f * sample;
    upsampleBufferVector[1] = 0.75f * previousSample + 0.25f * sample;
    upsampleBufferVector[2] = 0.625f * previousSample + 0.375f * sample;
    upsampleBufferVector[3] = 0.5f * (previousSample + sample);
    upsampleBufferVector[4] = 0.375f * previousSample + 0.625f * sample;
    upsampleBufferVector[5] = 0.25f * previousSample + 0.75f * sample;
    upsampleBufferVector[6] = 0.125f * previousSample + 0.875f * sample;
    upsampleBufferVector[7] = sample;
    previousSample = sample;

    upsampleBufferLength = 8;
}

void krakendsp::Distortion::applyFilterToBufferStereoWithVectors(bool inBuffer, bool isMono) {
    auto* bufferVectorPtr = inBuffer ? &aa2xInBufferVector : &aa2xOutBufferVector;
    auto* bufferIndexPtrVector = inBuffer ? &aa2xInBufferIndexVector : &aa2xOutBufferIndexVector;
    if (upsampleBufferLength == 4) {
        bufferVectorPtr = inBuffer ? &aa4xInBufferVector : &aa4xOutBufferVector;
        bufferIndexPtrVector = inBuffer ? &aa4xInBufferIndexVector : &aa4xOutBufferIndexVector;
    } else if (upsampleBufferLength == 8) {
        bufferVectorPtr = inBuffer ? &aa8xInBufferVector : &aa8xOutBufferVector;
        bufferIndexPtrVector = inBuffer ? &aa8xInBufferIndexVector : &aa8xOutBufferIndexVector;
    }

    for (size_t j = 0; j < upsampleBufferLength; j++) {
        bufferVectorPtr->at(*bufferIndexPtrVector) = {upsampleBufferVector[j].left, upsampleBufferVector[j].right};
        upsampleBufferVector[j] = {0, 0};
        if (inBuffer) {
            if (*bufferIndexPtrVector % 2 != 0) {
                for (size_t i = 0; i < halfFilterSize; i += 2) {
                    size_t tempIndex1Vec = (*bufferIndexPtrVector + aaFilterCoeffsVector.size() - i) % 256;
                    size_t tempIndex2Vec = (*bufferIndexPtrVector + i + 1) % 256;
                    upsampleBufferVector[j].left += (bufferVectorPtr->at(tempIndex1Vec).left + bufferVectorPtr->at(tempIndex2Vec).left) * aaFilterCoeffsVector[i];
                    if (!isMono) {
                        upsampleBufferVector[j].right += (bufferVectorPtr->at(tempIndex1Vec).right + bufferVectorPtr->at(tempIndex2Vec).right) * aaFilterCoeffsVector[i];
                    }
                }
            } else {
                upsampleBufferVector[j].left += bufferVectorPtr->at((*bufferIndexPtrVector - halfFilterSize + aaFilterCoeffsVector.size()) % 256).left * aaFilterCoeffsVector[halfFilterSize];
                if (!isMono) {
                    upsampleBufferVector[j].right += bufferVectorPtr->at((*bufferIndexPtrVector - halfFilterSize + aaFilterCoeffsVector.size()) % 256).right * aaFilterCoeffsVector[halfFilterSize];
                }
            }
        } else {
            for (size_t i = 0; i < halfFilterSize; i += 2) {

                size_t tempIndex1 = (*bufferIndexPtrVector + aaFilterCoeffsVector.size() - i) % 256;
                size_t tempIndex2 = (*bufferIndexPtrVector + i + 1) % 256;

                upsampleBufferVector[j].left += (bufferVectorPtr->at(tempIndex1).left + bufferVectorPtr->at(tempIndex2).left) * aaFilterCoeffsVector[i];
                if (!isMono) {
                    upsampleBufferVector[j].right += (bufferVectorPtr->at(tempIndex1).right + bufferVectorPtr->at(tempIndex2).right) * aaFilterCoeffsVector[i];
                }


            }
            upsampleBufferVector[j].left += bufferVectorPtr->at((*bufferIndexPtrVector - halfFilterSize + aaFilterCoeffsVector.size()) % 256).left * aaFilterCoeffsVector[halfFilterSize];
            if (!isMono) {
                upsampleBufferVector[j].right += bufferVectorPtr->at((*bufferIndexPtrVector - halfFilterSize + aaFilterCoeffsVector.size()) % 256).right * aaFilterCoeffsVector[halfFilterSize];
            }
        }
        *bufferIndexPtrVector = (*bufferIndexPtrVector + 1) % 256;
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

float krakendsp::Distortion::processRaw(float input) const {
    return input * gain;
}

float krakendsp::Distortion::processClean(float input) const {
    float sample = input * gain * 5.0f;
    if (sample < -3.f) { sample = -1.f; }
    else if (sample > 3.f) { sample = 1.f; }
    else { sample = sample * (27.f + sample*sample) / (27.f + sample * sample * 9.f); }
    return sample;
}

float krakendsp::Distortion::processSoftClip(float input) const {
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

float krakendsp::Distortion::processHardClip(float input) const {
    float sample = input * gain * gain * gain * 1000.0f;


    if (sample > 1) {
        sample = 1;
    } else if (sample < -1) {
        sample = -1;
    }
    return sample;
}

float krakendsp::Distortion::processWavefolder(float input) const {
    float polarity = 1;
    if (input < 0) {
        polarity = -1;
    }

    float sample = static_cast<float>(fabs(input));

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

void krakendsp::Distortion::fill2xOversampleBufferStereo(Stereo input) {
    upsampleBufferVector[0] = input;
    upsampleBufferVector[1] = {0.0f, 0.0f};
    
    upsampleBufferLength = 2;
}

void krakendsp::Distortion::fill4xOversampleBufferStereo() {
    upsampleBufferVector[0] = upsampleBufferVector[0];
    upsampleBufferVector[2] = upsampleBufferVector[1];
    upsampleBufferVector[1] = {0.0f, 0.0f};
    upsampleBufferVector[3] = {0.0f, 0.0f};

    upsampleBufferLength = 4;
}

void krakendsp::Distortion::fill8xOversampleBufferStereo() {
    upsampleBufferVector[0] = upsampleBufferVector[0];
    upsampleBufferVector[2] = upsampleBufferVector[1];
    upsampleBufferVector[4] = upsampleBufferVector[2];
    upsampleBufferVector[6] = upsampleBufferVector[3];
    upsampleBufferVector[1] = {0.0f, 0.0f};
    upsampleBufferVector[3] = {0.0f, 0.0f};
    upsampleBufferVector[5] = {0.0f, 0.0f};
    upsampleBufferVector[7] = {0.0f, 0.0f};
    
    upsampleBufferLength = 8;
}

void krakendsp::Distortion::decimate2xOversampleBufferStereo() {
    upsampleBufferVector[0] = upsampleBufferVector[0];
    upsampleBufferVector[1] = upsampleBufferVector[2];
    
    upsampleBufferLength = 2;
}

void krakendsp::Distortion::decimate4xOversampleBufferStereo() {
    upsampleBufferVector[0] = upsampleBufferVector[0];
    upsampleBufferVector[1] = upsampleBufferVector[2];
    upsampleBufferVector[2] = upsampleBufferVector[4];
    upsampleBufferVector[3] = upsampleBufferVector[6];
    
    upsampleBufferLength = 4;
}

void krakendsp::Distortion::setInterpolationUpsampleMode(bool _interpolationUpsample) {
    interpolationUpsample = _interpolationUpsample;
}

bool krakendsp::Distortion::getInterpolationUpsampleMode() const {
    return interpolationUpsample;
}

std::vector<krakendsp::Log> krakendsp::Distortion::getLog() const {
    return log;
}

void krakendsp::Distortion::setFilter1Freq(float freq) {
    filter1.setFrequency(freq);
}

void krakendsp::Distortion::setFilter1Type(size_t type) {
    filter1.setType(static_cast<int>(type));
}

float krakendsp::Distortion::getFilter1Freq() {
    return filter1.getControlValueRaw(0);
}

std::string krakendsp::Distortion::getFilter1Type() {
    auto type = filter1.getType();
    return type.name;
}

void krakendsp::Distortion::setFilter1Gain(float gain) {
    filter1.setGain(gain);
}

float krakendsp::Distortion::getFilter1Gain() {
    return filter1.getControlValueRaw(1);
}

void krakendsp::Distortion::setFilter1Q(float q) {
    filter1.setQ(q);
}

float krakendsp::Distortion::getFilter1Q() {
    return filter1.getControlValueRaw(2);
}

krakendsp::FileType krakendsp::Distortion::getFileType() const {
    return krakendsp::FileType::None;
}

std::vector<krakendsp::AcceptedFile> krakendsp::Distortion::getAcceptedFiles() const {
    std::vector<krakendsp::AcceptedFile> acceptedFiles;
    return acceptedFiles;
}

void krakendsp::Distortion::setFile(const krakendsp::FXFile& file, size_t fileIndex) {
    // Implementation for setting the file goes here
}