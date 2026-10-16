#ifndef KDSP_TREMOLO_H_
#define KDSP_TREMOLO_H_

#include "MultiFX.hpp"
#include "LFO.hpp"

namespace krakendsp
{
    class Tremolo : public MultiFX {
        public:
            Tremolo();
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

            FileType getFileType() const override;
            std::vector<AcceptedFile> getAcceptedFiles() const override;

            void setFile(const FXFile& file, size_t fileIndex) override;

            std::vector<Log> getLog() const override;

        private:
            float depth; // 0.0 to 1.0
            float rate;  // in Hz
            float sampleRate; // in Hz
            float rateNormalized;

            LFO lfo;

            std::vector<Log> log;

            std::string name = "Tremolo";
            std::vector<krakendsp::FXControl> controls = {
                krakendsp::FXControl{
                    .name ="Depth",
                    .displayMultiplier = 1.0f,
                    .unit = "",
                    .valueMin = 0.0f,
                    .valueMax = 1.0f,
                    .index = 0},
                krakendsp::FXControl{
                    .name = "Rate",
                    .displayMultiplier = 10.0f,
                    .unit = "Hz",
                    .valueMin = 0.01f,
                    .valueMax = 1.0f,
                    .index = 1},
                krakendsp::FXControl{
                    .name = "Horizontal Distortion",
                    .displayMultiplier = 1.0f,
                    .unit = "",
                    .valueMin = 0.01f,
                    .valueMax = 0.99f,
                    .index = 2},
                krakendsp::FXControl{
                    .name = "Vertical Distortion",
                    .displayMultiplier = 1.0f,
                    .unit = "",
                    .valueMin = 0.01f,
                    .valueMax = 0.99f,
                    .index = 3},
                krakendsp::FXControl{
                    .name = "LFO smoothing",
                    .displayMultiplier = 1.0f,
                    .unit = "",
                    .valueMin = 0.0f,
                    .valueMax = 1.0f,
                    .index = 4}
            };

            std::vector<krakendsp::FXSubType> types = {
                krakendsp::FXSubType{
                    .name = "Sin",
                    .typeID = 0
                },
                krakendsp::FXSubType{
                    .name = "Square",
                    .typeID = 1
                },
                krakendsp::FXSubType{
                    .name = "Triangle",
                    .typeID = 2
                },
                krakendsp::FXSubType{
                    .name = "Sawtooth",
                    .typeID = 3
                }
            };
    };
}
#endif // KDSP_TREMOLO_H_
