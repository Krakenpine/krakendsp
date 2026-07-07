#ifndef KDSP_DISTORTION_H_
#define KDSP_DISTORTION_H_

#include "MultiFX.hpp"
#include <vector>
#include <string>
#include <array>

namespace krakendsp
{
    class Distortion : public krakendsp::MultiFX
    {
        public:
            Distortion() = default;
            Distortion(float gain_, int type, float oversample_, float samplerate_);
            ~Distortion() override = default;

            float process(float sample) override;
            Stereo process(Stereo sample) override;
            
            std::string getName() const override;
            std::vector<krakendsp::FXSubType> getTypes() const override;
            krakendsp::FXSubType getType() const override;
            krakendsp::FXSubType getType(int index) const override;
            std::vector<krakendsp::FXControl> getControls() const override;
            krakendsp::FXControl getControl(int index) const override;
            float getControlValue(int index) const override;
            float getControlValueRaw(int index) const override;

            float getGain() const;
            float getOversample() const;
            float getSamplerate() const;
            
            void setType(int index) override;
            void setControl(int index, float value) override;

            void setGain(float gain_);
            void setOversample(float oversample_);
            void setSampleRate(float samplerate_);

            std::vector<Log> getLog() const override;
            
        private:
            float gain = 0;
            float samplerate = 48000;
            float oversample = 1;
            int currentType = 0;

            std::vector<Log> log;

            std::string name = "Distortion";
            std::vector<krakendsp::FXControl> controls = {
                krakendsp::FXControl{"Gain", 1.0f, "", 0.0f, 1.0f, 0}
            };
            std::vector<krakendsp::FXSubType> types = {
                krakendsp::FXSubType{"Raw", 0},
                krakendsp::FXSubType{"Softclip", 1},
                krakendsp::FXSubType{"Hardclip", 2},
                krakendsp::FXSubType{"MetalZone", 3},
                krakendsp::FXSubType{"Wavefolder", 4},
                krakendsp::FXSubType{"Clean", 5}
            };

            float processRaw(float input);
            float processClean(float input);
            float processSoftClip(float input);
            float processHardClip(float input);
            float processWavefolder(float input);

            float processHardClipNonAliased(float input);

            float prevValue = 0;
            float prevOutput = 0;
            float prevFiltInput = 0;
    };
}

#endif // KDSP_DISTORTION_H_