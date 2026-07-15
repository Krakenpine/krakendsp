#ifndef KDSP_MULTIFX_H_
#define KDSP_MULTIFX_H_

#include <vector>
#include <string>

namespace krakendsp
{

struct FXControl {
    std::string name;
    float displayMultiplier;
    std::string unit;
    float valueMin;
    float valueMax;
    int index;
};

struct FXSubType {
    std::string name;
    int typeID;
};

struct Stereo {
    float left;
    float right;
};

struct Log {
    std::string text;
    float value;
};

inline float scaleValue(float min, float max, float normalizedValue) {
    float scaledValue = (max - min) * normalizedValue + min;
    return scaledValue;
}

inline float samplesToMS(float samples, float samplerate) {
    return samples * 1000.0f / samplerate;
}

inline float MSToSamples(float ms, float samplerate) {
    return ms * samplerate / 1000.0f;
}


class MultiFX {
    public:
        virtual ~MultiFX() = default;

        virtual float process(float input) = 0;
        virtual Stereo process(Stereo input) = 0;

        virtual std::string getName() const = 0;
        virtual std::vector<FXSubType> getTypes() const = 0;
        virtual FXSubType getType() const = 0;
        virtual FXSubType getType(int index) const = 0;
        virtual std::vector<FXControl> getControls() const = 0;
        virtual FXControl getControl(int index) const = 0;
        virtual float getControlValue(int index) const = 0;
        virtual float getControlValueRaw(int index) const = 0;
        
        virtual void setType(int index) = 0;
        virtual void setControl(int index, float value) = 0;

        virtual std::vector<Log> getLog() const = 0;
};
}

#endif // KDSP_MULTIFX_H_