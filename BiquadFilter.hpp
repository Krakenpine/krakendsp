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
            krakendsp::FXSubType getType(int index) const override;
            std::vector<krakendsp::FXControl> getControls() const override;
            krakendsp::FXControl getControl(int index) const override;
            float getControlValue(int index) const override;
            float getControlValueRaw(int index) const override;

            void setSamplerate(float samplerate_);
            void setFrequency(float frequency_);
            void setGain(float gain_);
            void setQ(float q_);

            void setType(int index) override;
            void setControl(int index, float value) override;

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

            std::string name = "Biquad filter";
            std::vector<krakendsp::FXControl> controls = {
                krakendsp::FXControl{
                    .name = "Frequency",
                    .displayMultiplier = 1.0f,
                    .unit = "Hz",
                    .valueMin = 1.0f,
                    .valueMax = 20000.0f,
                    .index = 0
                },
                krakendsp::FXControl{
                    .name = "Gain",
                    .displayMultiplier = 1.0f,
                    .unit = "dB",
                    .valueMin = 0.0f,
                    .valueMax = 1.0f,
                    .index = 1
                },
                krakendsp::FXControl{
                    .name = "Q",
                    .displayMultiplier = 1.0f,
                    .unit = "",
                    .valueMin = 0.0f,
                    .valueMax = 10.0f,
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