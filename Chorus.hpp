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
            Chorus() = default;
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
                {"Rate", 10.0f, "Hz", 0.1f, 10.0f, 0},
                {"Depth", 50.0f, "ms", 5.0f, 50.0f, 1},
                {"Min depth", 50.0f, "ms", 1.0f, 20.0f, 2},
                {"Mix", 1.0f, "", 0.0f, 1.0f, 3},
                {"Feedback", 1.0f, "", 0.0f, 1.0f, 4}
            };

            std::vector<krakendsp::FXSubType> types = {
                krakendsp::FXSubType{"Standard", 0}
            };

            int type = 0;
    };
}

#endif // KDSP_CHORUS_H_