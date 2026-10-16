#ifndef KDSP_CHORUS_H_
#define KDSP_CHORUS_H_

#include "MultiFX.hpp"
#include "Delay.hpp"
#include "RingBuffer.hpp"
#include "LFO.hpp"
#include <vector>
#include <string>
#include <array>

namespace krakendsp
{
    class Chorus : public krakendsp::MultiFX
    {
        public:
            Chorus(float rate_, float depth_, float minDepth_, float mix_, float feedback_, float samplerate_, size_t bufferSize_, float* buffer1_, float* buffer2_, bool isMono_);
            ~Chorus() override = default;

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

            float getRate() const;
            float getDepth() const;
            float getMinDepth() const;
            float getMix() const;
            float getFeedback() const;
            float getSamplerate() const;

            float getRateNonNormalized() const;
            float getDepthNonNormalized() const;
            float getMinDepthNonNormalized() const;

            void setType(int index) override;
            void setControl(int index, float value) override;

            void setRate(float rate_);
            void setDepth(float depth_);
            void setMinDepth(float minDepth_);
            void setMix(float mix_);
            void setFeedback(float feedback_);
            void setSamplerate(float samplerate_);

            FileType getFileType() const override;
            std::vector<AcceptedFile> getAcceptedFiles() const override;

            void setFile(const FXFile& file, size_t fileIndex) override;

            std::vector<Log> getLog() const override;

        private:
            float rate;
            float rateNormalized;
            float depth;
            float depthNormalized;
            float minDepth;
            float minDepthNormalized;
            float mix;
            float feedback;
            float samplerate;

            float delayMinLengthSamples;
            float delayMaxLengthSamples;

            bool isMono;

            LFO lfo;
            Delay delay1;
            Delay delay2;

            std::vector<Log> log;

            std::string name = "Chorus";
            std::vector<krakendsp::FXControl> controls = {
                krakendsp::FXControl{
                    .name = "Rate",
                    .displayMultiplier = 10.0f,
                    .unit = "Hz",
                    .valueMin = 0.1f,
                    .valueMax = 10.0f,
                    .index = 0},
                krakendsp::FXControl{
                    .name = "Depth",
                    .displayMultiplier = 50.0f,
                    .unit = "ms",
                    .valueMin = 5.0f,
                    .valueMax = 50.0f,
                    .index = 1
                },
                krakendsp::FXControl{
                    .name = "Min depth",
                    .displayMultiplier = 50.0f,
                    .unit = "ms",
                    .valueMin = 1.0f,
                    .valueMax = 20.0f,
                    .index = 2
                },
                krakendsp::FXControl{
                    .name = "Mix",
                    .displayMultiplier = 1.0f,
                    .unit = "",
                    .valueMin = 0.0f,
                    .valueMax = 1.0f,
                    .index = 3
                },
                krakendsp::FXControl{
                    .name = "Feedback",
                    .displayMultiplier = 1.0f,
                    .unit = "",
                    .valueMin = 0.0f,
                    .valueMax = 1.0f,
                    .index = 4
                }
            };

            std::vector<krakendsp::FXSubType> types = {
                krakendsp::FXSubType{
                    .name = "Standard",
                    .typeID = 0
                }
            };

            int type = 0;
    };
}

#endif // KDSP_CHORUS_H_