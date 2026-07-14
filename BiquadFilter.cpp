#include "BiquadFilter.hpp"

krakendsp::BiquadFilter::BiquadFilter() {
    calculateCoeffs();
}

krakendsp::BiquadFilter::BiquadFilter(float samplerate_) : 
    samplerate(samplerate_) {
        calculateCoeffs();
    }

void krakendsp::BiquadFilter::setSamplerate(float samplerate_) {
    samplerate = samplerate_;
    Fs = 1.0f / samplerate;
    calculateCoeffs();
}

void krakendsp::BiquadFilter::setFrequency(float frequency_) {
    frequency = frequency_;
    if (frequency < 10.0f) {
        frequency = 10.0f;
    }
    calculateCoeffs();
}

void krakendsp::BiquadFilter::setGain(float gain_) {
    gain = gain_;
    calculateCoeffs();
}

void krakendsp::BiquadFilter::setQ(float q_) {
    q = q_;
    calculateCoeffs();
}

std::string krakendsp::BiquadFilter::getName() const {
    return name;
}

float krakendsp::BiquadFilter::process(float sample) {
    x2.left = x1.left;
    x1.left = x0.left;
    x0.left = sample;
    float y0L = (b0/a0)*x0.left + (b1/a0)*x1.left + (b2/a0)*x2.left - (a1/a0)*y1.left - (a2/a0)*y2.left;
    y2.left = y1.left;
    y1.left = y0L;
    return y0L;
}

krakendsp::Stereo krakendsp::BiquadFilter::process(Stereo sample) {
    Stereo y0;
    x2.left = x1.left;
    x1.left = x0.left;
    x0.left = sample.left;
    y0.left = (b0/a0)*x0.left + (b1/a0)*x1.left + (b2/a0)*x2.left - (a1/a0)*y1.left - (a2/a0)*y2.left;
    y2.left = y1.left;
    y1.left = y0.left;

    x2.right = x1.right;
    x1.right = x0.right;
    x0.right = sample.right;
    y0.right = (b0/a0)*x0.right + (b1/a0)*x1.right + (b2/a0)*x2.right - (a1/a0)*y1.right - (a2/a0)*y2.right;
    y2.right = y1.right;
    y1.right = y0.right;

    return y0;
}

void krakendsp::BiquadFilter::calculateCoeffs() {
    switch (currentType) {
        case 0:
            calculateLPFCoeffs();
            break;
        case 1:
            calculateHPFCoeffs();
            break;
        case 2:
            calculateBPCoeffs();
            break;
        case 3:
            calculatePeakCoeffs();
            break;
        case 4:
            calculateLSCoeffs();
            break;
        case 5:
            calculateHSCoeffs();
            break;
        default:
            calculateLPFCoeffs();
            break;
    }
}

void krakendsp::BiquadFilter::calculateLPFCoeffs() {
    float A = pow(10.0f, (gain/40.0f));
    omega = 2.0f * 3.14f * (frequency / samplerate);
    alpha = sin(omega) / (2.0f * q);
    float cosOmega = cos(omega);

    b0 = (1.0f - cosOmega) / 2.0f;
    b1 = 1.0f - cosOmega;
    b2 = (1.0f - cosOmega) / 2.0f;
    a0 = 1.0f + alpha;
    a1 = -2.0f * cosOmega;
    a2 = 1.0f - alpha;
}

void krakendsp::BiquadFilter::calculateHPFCoeffs() {
    float A = pow(10.0f, (gain/40.0f));
    omega = 2.0f * 3.14f * (frequency / samplerate);
    alpha = sin(omega) / (2.0f * q);
    float cosOmega = cos(omega);

    b0 = (1.0f + cosOmega) / 2.0f;
    b1 = -1.0f * (1.0f + cosOmega);
    b2 = (1.0f + cosOmega) / 2.0f;
    a0 = 1.0f + alpha;
    a1 = -2.0f * cosOmega;
    a2 = 1.0f - alpha;
}

void krakendsp::BiquadFilter::calculateBPCoeffs() {
    float A = pow(10.0f, (gain/40.0f));
    omega = 2.0f * 3.14f * (frequency / samplerate);
    alpha = sin(omega) / (2.0f * q);
    float cosOmega = cos(omega);

    b0 = alpha;
    b1 = 0.0f;
    b2 = -1.0f * alpha;
    a0 = 1.0f + alpha;
    a1 = -2.0f * cosOmega;
    a2 = 1.0f - alpha;
}

void krakendsp::BiquadFilter::calculatePeakCoeffs() {
    float A = pow(10.0f, (gain/40.0f));
    omega = 2.0f * 3.14f * (frequency / samplerate);
    alpha = sin(omega) / (2.0f * q);
    float cosOmega = cos(omega);

    b0 = 1.0f + alpha * A;
    b1 = -2.0f * cosOmega;
    b2 = 1.0f - alpha * A;
    a0 = 1.0f + (alpha / A);
    a1 = -2.0f * cosOmega;
    a2 = 1.0f - (alpha / A);
}

void krakendsp::BiquadFilter::calculateLSCoeffs() {
    float A = pow(10.0f, (gain/40.0f));
    omega = 2.0f * 3.14f * (frequency / samplerate);
    alpha = sin(omega) / (2.0f * q);
    float cosOmega = cos(omega);

    b0 = A * ((A + 1.0f) - (A - 1.0f) * cosOmega + 2.0f * sqrt(A) * alpha);
    b1 = 2.0f * A * ((A - 1.0f) - (A + 1.0f) * cosOmega);
    b2 = A * ((A + 1.0f) - (A - 1.0f) * cosOmega - 2.0f * sqrt(A) * alpha);
    a0 = (A + 1.0f) + (A - 1.0f) * cosOmega + 2.0f * sqrt(A) * alpha;
    a1 = -2.0f * ((A - 1.0f) + (A + 1.0f) * cosOmega);
    a2 = (A + 1.0f) + (A - 1.0f) * cosOmega - 2.0f * sqrt(A) * alpha;
}

void krakendsp::BiquadFilter::calculateHSCoeffs() {
    float A = pow(10.0f, (gain/40.0f));
    omega = 2.0f * 3.14f * (frequency / samplerate);
    alpha = sin(omega) / (2.0f * q);
    float cosOmega = cos(omega);

    b0 = A * ((A + 1.0f) + (A - 1.0f) * cosOmega + 2.0f * sqrt(A) * alpha);
    b1 = -2.0f * A * ((A - 1.0f) + (A + 1.0f) * cosOmega);
    b2 = A * ((A + 1.0f) + (A - 1.0f) * cosOmega - 2.0f * sqrt(A) * alpha);
    a0 = (A + 1.0f) - (A - 1.0f) * cosOmega + 2.0f * sqrt(A) * alpha;
    a1 = 2.0f * ((A - 1.0f) - (A + 1.0f) * cosOmega);
    a2 = (A + 1.0f) - (A - 1.0f) * cosOmega - 2.0f * sqrt(A) * alpha;    
}

void krakendsp::BiquadFilter::setControl(int index, float value) {
    switch (index) {
        case 0: setFrequency(value); break;
        case 1: setGain(value); break;
        case 2: setQ(value); break;
        default: break;
    }
}

float krakendsp::BiquadFilter::getControlValue(int index) const {
    switch (index) {
        case 0: return frequency;
        case 1: return gain;
        case 2: return q;
        default: return 0;
    }
}

krakendsp::FXControl krakendsp::BiquadFilter::getControl(int index) const {
    switch (index) {
        case 0: return controls[0];
        case 1: return controls[1];
        case 2: return controls[2];
        default: return krakendsp::FXControl{};
    }
}

float krakendsp::BiquadFilter::getControlValueRaw(int index) const {
    switch (index) {
        case 0: return frequency;
        case 1: return gain;
        case 2: return q;
        default: return 0.0f;
    }
}

krakendsp::FXSubType krakendsp::BiquadFilter::getType(int index) const {
    return types[index];
}

krakendsp::FXSubType krakendsp::BiquadFilter::getType() const {
    return types[currentType];
}

void krakendsp::BiquadFilter::setType(int index) {
    if (index >= 0 && index < static_cast<int>(types.size())) {
        currentType = index;
        calculateCoeffs();
    }
}

std::vector<krakendsp::FXControl> krakendsp::BiquadFilter::getControls() const {
    return controls;
}

std::vector<krakendsp::FXSubType> krakendsp::BiquadFilter::getTypes() const {
    return types;
}

std::vector<krakendsp::Log> krakendsp::BiquadFilter::getLog() const {
    return log;
}