#include "CabinetModeler.hpp"

krakendsp::CabinetModeler::CabinetModeler(float samplerate_, bool isMono_) : 
    samplerate(samplerate_),
    isMono(isMono_) {
        IRsum = 0.0f;
        for (int i = 0; i < 1115; i++) {
            FourByTwelve57CloseFloat[i] = static_cast<float>(FourByTwelve57Close[i]) / 32767.0f;
            IRsum += fabs(FourByTwelve57CloseFloat[i]);
        }
    }

float krakendsp::CabinetModeler::process(float sample) {
    audiobuffer[bufferIndex] = sample;
    float tempSample = 0.0f;
    for (size_t i = 0; i < IRlength; i++) {
        size_t tempIndex = (bufferIndex - i + 65536) % 65536;
        tempSample += FourByTwelve57CloseFloat[i] * audiobuffer[tempIndex];
    }

    bufferIndex = (bufferIndex + 1) % 65536;

    return tempSample / IRsum;
}

krakendsp::Stereo krakendsp::CabinetModeler::process(Stereo sample) {
    Stereo output = {0.0f, 0.0f};
    output.left = process(sample.left);
    output.right = output.left;
    return output;
}

std::string krakendsp::CabinetModeler::getName() const {
    return name;
}

std::vector<krakendsp::FXSubType> krakendsp::CabinetModeler::getTypes() const {
    std::vector<krakendsp::FXSubType> typesFake = {};
    return typesFake;
}

krakendsp::FXSubType krakendsp::CabinetModeler::getType() const {
    krakendsp:FXSubType fakeType;
    return fakeType;
}

krakendsp::FXSubType krakendsp::CabinetModeler::getType(int index) const {
    krakendsp:FXSubType fakeType;
    return fakeType;
}

std::vector<krakendsp::FXControl> krakendsp::CabinetModeler::getControls() const {
    std::vector<krakendsp::FXControl> fakeControls = {};
    return fakeControls;
}

krakendsp::FXControl krakendsp::CabinetModeler::getControl(int index) const {
    krakendsp::FXControl fakeControl;
    return fakeControl;
}

float krakendsp::CabinetModeler::getControlValue(int index) const {
    return 0.0f;
}

float krakendsp::CabinetModeler::getControlValueRaw(int index) const {
    return 0.0f;
}

void krakendsp::CabinetModeler::setControl(int index, float value) {

}

void krakendsp::CabinetModeler::setType(int index) {

}

std::vector<krakendsp::Log> krakendsp::CabinetModeler::getLog() const {
    return log;
}

void krakendsp::CabinetModeler::setIRLength(float IRlength_) {
    IRlength = static_cast<size_t>(IRlength_ * 1115.0f);
}