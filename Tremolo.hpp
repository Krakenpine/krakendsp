#ifndef KDSP_TREMOLO_H_
#define KDSP_TREMOLO_H_

#include "MultiFX.hpp"
#include "LFO.hpp"

namespace krakendsp
{
    class Tremolo : public MultiFX {
        public:
            Tremolo() = default;
            Tremolo(float depth_, float rate_, float sampleRate_);
            ~Tremolo() override = default;

            float process(float input) override;
            Stereo process(Stereo input) override;
            
            std::string getName() const override;
            std::vector<FXSubType> getTypes() const override;
            FXSubType getType() const override;
            FXSubType getType(int index) const override;
            std::vector<FXControl> getControls() const override;
            FXControl getControl(int index) const override;
            float getControlValue(int index) const override;
            float getControlValueRaw(int index) const override;

            void setType(int index) override;
            void setControl(int index, float value) override;

            void setDepth(float depth_);
            void setRate(float rate_);
            void setLFOType(LFOType type_);
            void setSampleRate(float sampleRate_);

            std::vector<Log> getLog() const override;

        private:
            float depth; // 0.0 to 1.0
            float rate;  // in Hz
            float sampleRate; // in Hz
            float rateNormalized;

            LFO lfo;

            std::vector<Log> log;

            std::string name = "Tremolo";
            std::vector<FXControl> controls = {
                {"Depth", 1.0f, "", 0.0f, 1.0f, 0},
                {"Rate", 10.0f, "Hz", 0.01f, 1.0f, 1},
                {"Horizontal Distortion", 1.0f, "", 0.01f, 0.99f, 2},
                {"Vertical Distortion", 1.0f, "", 0.01f, 0.99f, 3},
                {"LFO smoothing", 1.0f, "", 0.0f, 1.0f, 4}
            };

            std::vector<FXSubType> types = {
                {"Sin", 0},
                {"Square", 1},
                {"Triangle", 2},
                {"Sawtooth", 3}
            };
    };
}
#endif // KDSP_TREMOLO_H_
