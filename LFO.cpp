#include "LFO.hpp"
#include <cmath>
#include <algorithm>
#include <numbers>

constexpr float M_PI2 = 3.14159265358979323846f;
// constexpr float M_PI = std::numbers::pi_v<float>;
// VScode intellisense does not support all C++20 features?

krakendsp::LFO::LFO(float frequency_, float sampleRate_) 
    : frequency(frequency_),
    sampleRate(sampleRate_),
    phaseIncrement(frequency / sampleRate) { }

krakendsp::LFO::LFO(float frequency_, float sampleRate_, size_t sinTableSize_, float* sinTable_) 
    : frequency(frequency_),
    sampleRate(sampleRate_),
    sinTableSize(sinTableSize_),
    sinTable(sinTable_),
    phaseIncrement(frequency / sampleRate) { }

void krakendsp::LFO::setFrequency(float frequency_) {
    frequency = frequency_;
    phaseIncrement = frequency / sampleRate;
}

void krakendsp::LFO::setSampleRate(float sampleRate_) {
    sampleRate = sampleRate_;
    phaseIncrement = frequency / sampleRate;
}

void krakendsp::LFO::setType(LFOType type_) {
    type = type_;
}

void krakendsp::LFO::setVerticalDistort(float verticalDistort_) {
    verticalDistort = std::clamp(verticalDistort_, 0.01f, 0.99f);
}

void krakendsp::LFO::setHorizontalDistort(float horizontalDistort_) {
    horizontalDistort = std::clamp(horizontalDistort_, 0.01f, 0.99f);
}

float krakendsp::LFO::getFrequency() const {
    return frequency;
}

float krakendsp::LFO::getSampleRate() const {
    return sampleRate;
}

float fastTanh(float x) {
    // Fast tanh approximation
    if (x < -3.0f) return -1.0f;
    if (x > 3.0f) return 1.0f;
    return x * (27.0f + x * x) / (27.0f + x * x * 9.0f);
}

float krakendsp::LFO::getSin(float phase_) {
    if (sinTableSize > 0 && sinTable != nullptr) {
        // TO DO: interpolate between table values for better accuracy
        size_t index = static_cast<size_t>(phase_ * static_cast<float>(sinTableSize)) % sinTableSize;
        return sinTable[index];
    } else {
        return sin(2.0f * M_PI2 * phase_);
    }
}

float krakendsp::LFO::getNextSample() {
    float sample = 0.0f;

    float tempPhase = phase;

    if (phase > horizontalDistort) {
        tempPhase =  0.5f + (phase - horizontalDistort) * (0.5f / (1.0f - horizontalDistort));
    } else {
        tempPhase = (0.5f * phase) / horizontalDistort;
    }

    switch (type) {
        case krakendsp::LFOType::SINE:
            sample = sin(2.0f * M_PI2 * tempPhase);
            break;
        case krakendsp::LFOType::SQUARE:
            sample = (tempPhase > 0.5f) ? 1.0f : -1.0f;
            break;
        case krakendsp::LFOType::TRIANGLE:
            sample = 2.0f * fabs(2.0f * (tempPhase - floor(tempPhase + 0.5f))) - 1.0f;
            break;
        case krakendsp::LFOType::SAWTOOTH:
            sample = 2.0f * tempPhase - 1.0f;
            break;
    }

    if (verticalDistort > 0.5f) {
        float verticalFactor = (verticalDistort - 0.5f) * 2.0f;
        if (sample > 0.0f) {
            sample = sample * (1-verticalFactor) + verticalFactor;
        } else {
            sample = sample * (1 + verticalFactor) + verticalFactor;
        }
    } else {
        float verticalFactor = (0.5f - verticalDistort) * 2.0f;
        if (sample > 0.0f) {
            sample = sample * (1-verticalFactor) + verticalFactor;
        } else {
            sample = sample * (1 + verticalFactor) + verticalFactor;
        }
    }

    phase += phaseIncrement;

    if (phase >= 1.0f) {
        phase -= 1.0f;
    }

    float slewRateDelta = filterFreqInv;

    if (sample > prevSample && abs(sample - prevSample) > slewRateDelta) {
        sample = prevSample + slewRateDelta;
    } else if (sample < prevSample && abs(prevSample - sample) > slewRateDelta) {
        sample = prevSample - slewRateDelta;
    }

    prevSample = sample;

    return sample;
}

void krakendsp::LFO::setFilter(float filterFreq_) {
    float slewMagicNumber = 0.01f;
    float slewLimit = 0.0001f;
    filterFreq = filterFreq_;
    if (filterFreq < 0.01f) {
        filterFreqInv = 1.0f;
    } else {
        filterFreqInv = slewLimit + slewMagicNumber - filterFreq * filterFreq * filterFreq * slewMagicNumber;
    }
}

float krakendsp::LFO::getFilterFreq() const {
    return filterFreq;
}

float krakendsp::LFO::getNextSampleUnipolar() {
    float sample = 0.5f * getNextSample() + 0.5f; // Convert to unipolar range
    return sample;
}

float krakendsp::LFO::getHorizontalDistort() const {
    return horizontalDistort;
}

float krakendsp::LFO::getVerticalDistort() const {
    return verticalDistort;
}

krakendsp::LFOType krakendsp::LFO::getType() const {
    return type;
}