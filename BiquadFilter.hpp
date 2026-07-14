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

            Stereo x0 = {0.0f, 0.0f};
            Stereo x1 = {0.0f, 0.0f};
            Stereo x2 = {0.0f, 0.0f};
            Stereo y1 = {0.0f, 0.0f};
            Stereo y2 = {0.0f, 0.0f};

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
                krakendsp::FXControl{"Frequency", 1.0f, "Hz", 1.0f, 20000.0f, 0},
                krakendsp::FXControl{"Gain", 1.0f, "dB", 0.0f, 1.0f, 1},
                krakendsp::FXControl{"Q", 1.0f, "", 0.0f, 10.0f, 2}
            };
            std::vector<krakendsp::FXSubType> types = {
                krakendsp::FXSubType{"Lowpass", 0},
                krakendsp::FXSubType{"Highpass", 1},
                krakendsp::FXSubType{"Bandpass", 2},
                krakendsp::FXSubType{"Peak", 3},
                krakendsp::FXSubType{"Lowshelf", 4},
                krakendsp::FXSubType{"Highshelf", 5}
            };

    };

}

#endif // KDSP_BIQUAD_FILTER_H_