#ifndef KDSP_DELAY_H_
#define KDSP_DELAY_H_

#include "MultiFX.hpp"
#include "RingBuffer.hpp"
#include <vector>
#include <string>
#include <array>

namespace krakendsp
{
    class Delay : public krakendsp::MultiFX
    {
        public:
            Delay() = default;
            Delay(float feedback_, float mix_, float length_, float samplerate_, size_t bufferSize_, bool isMono_, bool dynamicBuffer_, float* buffer1_, float* buffer2_);
            ~Delay() override = default;

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

            float getLength() const;
            float getLengthNormalized() const;
            float getLengthMS() const;
            float getFeedback() const;
            float getMix() const;
            size_t getBufferSize() const;
            float getSamplerate() const;

            void setType(int index) override;
            void setControl(int index, float value) override;

            void setLength(float newLength);
            void setLengthNormalized(float newLengthNormalized);
            void setLengthMS(float newLengthMS);
            void setFeedback(float feedback_);
            void setMix(float mix_);
            void setSamplerate(float samplerate_);

            std::vector<Log> getLog() const override;

        private:
            float feedback = 0;
            float mix = 0;
            float samplerate = 44100.0f;

            bool dynamicBuffer;

            size_t bufferSize;

            RingBuffer ringBuffer1;
            RingBuffer ringBuffer2;

            float* buffer1;
            float* buffer2;

            float delayLength;
            float delayLengthNormalized;

            bool isMono;

            std::vector<Log> log;

            std::string name = "Delay";
            std::vector<krakendsp::FXControl> controls = {
                krakendsp::FXControl{"Feedback", 1.0f, "", 0.0f, 1.0f, 0},
                krakendsp::FXControl{"Mix", 1.0f, "", 0.0f, 1.0f, 1},
                krakendsp::FXControl{"Length", samplerate, "ms", 0.0f, 1.0f, 2}
            };

            std::vector<FXSubType> types = {
                krakendsp::FXSubType{"Standard", 0}
            };

            float samplesToMS(float samples) const;
            float MSToSamples(float ms) const;

    };
}


#endif // KDSP_DELAY_H_