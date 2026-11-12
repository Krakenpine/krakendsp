#ifndef KDSP_BIQUAD_FILTER_H_
#define KDSP_BIQUAD_FILTER_H_

#include "MultiFX.hpp"
#include <vector>
#include <string>
#include <array>

namespace krakendsp
{
    class BiquadFilter : public krakendsp::MultiFX
    {
        public:
            BiquadFilter();
            BiquadFilter(float samplerate_);
            ~BiquadFilter() override = default;

            float process(float sample) override;
            Stereo process(Stereo sample) override;

            std::string getName() const override;
            std::vector<krakendsp::FXSubType> getTypes() const override;
            krakendsp::FXSubType getType() const override;
            krakendsp::FXSubType getType(unsigned char index) const override;
            std::vector<krakendsp::FXControl> getControls() const override;
            krakendsp::FXControl getControl(unsigned char index) const override;
            float getControlValue(unsigned char index) const override;
            float getControlValueRaw(unsigned char index) const override;

            void setSamplerate(float samplerate_);
            void setFrequency(float frequency_);
            float setFrequencyNormalized(float frequencyNormalized_);
            void setGain(float gain_);
            float setGainNormalized(float gainNormalized_);
            void setQ(float q_);
            float setQNormalized(float qNormalized_);

            void setType(unsigned char index) override;
            void setControl(unsigned char index, float value) override;
            float setControlNormalized(unsigned char index, float value) override;

            FileType getFileType() const override;
            std::vector<AcceptedFile> getAcceptedFiles() const override;
            void setFile(const FXFile& file, size_t fileIndex) override;

            std::vector<Log> getLog() const override;

        private:
            float samplerate = 44100.0f;
            int currentType = 0;

            float frequency = 1000.0f;
            float gain = 0.0f;
            float q = 0.707f;

            float frequencyNormalized = 0.0f;
            float gainNormalized = 0.0f;
            float qNormalized = 0.0f;

            float Fs = 1.0f / 44100.0f;

            float omega = 0.0f;
            float alpha = 0.0f;

            float a0 = 1.0f;
            float a1 = 1.0f;
            float a2 = 1.0f;
            float b0 = 1.0f;
            float b1 = 1.0f;
            float b2 = 1.0f;

            Stereo x0;
            Stereo x1;
            Stereo x2;
            Stereo y1;
            Stereo y2;

            void calculateCoeffs();
            void calculateLPFCoeffs();
            void calculateHPFCoeffs();
            void calculateBPCoeffs();
            void calculatePeakCoeffs();
            void calculateLSCoeffs();
            void calculateHSCoeffs();

            std::vector<Log> log;

            float frequencyMin = 10.0f;
            float frequencyMax = 20000.0f;
            float gainMin = -10.0f;
            float gainMax = 10.0f;
            float qMin = 0.0f;
            float qMax = 10.0f;

            std::string name = "Biquad filter";
            std::vector<krakendsp::FXControl> controls = {
                krakendsp::FXControl{
                    .name = "Frequency",
                    .displayMultiplier = frequencyMax - frequencyMin,
                    .unit = "Hz",
                    .valueMin = frequencyMin,
                    .valueMax = frequencyMax,
                    .index = 0
                },
                krakendsp::FXControl{
                    .name = "Gain",
                    .displayMultiplier = gainMax - gainMin,
                    .unit = "dB",
                    .valueMin = gainMin,
                    .valueMax = gainMax,
                    .index = 1
                },
                krakendsp::FXControl{
                    .name = "Q",
                    .displayMultiplier = qMax - qMin,
                    .unit = "",
                    .valueMin = qMin,
                    .valueMax = qMax,
                    .index = 2
                }
            };
            std::vector<krakendsp::FXSubType> types = {
                krakendsp::FXSubType{ .name = "Lowpass",   .typeID = 0},
                krakendsp::FXSubType{ .name = "Highpass",  .typeID = 1},
                krakendsp::FXSubType{ .name = "Bandpass",  .typeID = 2},
                krakendsp::FXSubType{ .name = "Peak",      .typeID = 3},
                krakendsp::FXSubType{ .name = "Lowshelf",  .typeID = 4},
                krakendsp::FXSubType{ .name = "Highshelf", .typeID = 5}
            };

    };

}

#endif // KDSP_BIQUAD_FILTER_H_