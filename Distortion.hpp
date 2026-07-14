#ifndef KDSP_DISTORTION_H_
#define KDSP_DISTORTION_H_

#include "MultiFX.hpp"
#include <vector>
#include <string>
#include <array>
#include "BiquadFilter.hpp"

namespace krakendsp
{
    enum class Oversample {
        None = 1,
        x2 = 2,
        x4 = 4,
        x8 = 8
    };
    
    class Distortion : public krakendsp::MultiFX
    {
        public:
            Distortion() = default;
            Distortion(float gain_, int type, size_t oversample_, float samplerate_);
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
            size_t getOversample() const;
            float getSamplerate() const;
            
            void setType(int index) override;
            void setControl(int index, float value) override;

            void setGain(float gain_);
            void setOversample(size_t oversample_);
            void setSampleRate(float samplerate_);
            void setInterpolationUpsampleMode(bool _interpolationUpsample);

            bool getInterpolationUpsampleMode();

            std::vector<Log> getLog() const override;

            void setFilter1Freq(float freq);
            void setFilter1Type(size_t type);
            float getFilter1Freq();
            std::string getFilter1Type();

            void setFilter1Gain(float gain);
            float getFilter1Gain();

            void setFilter1Q(float q);
            float getFilter1Q();
            
        private:
            float gain = 0;
            float samplerate = 44100;
            Oversample oversample = Oversample::None;
            int currentType = 0;

            std::vector<Log> log;

            std::string name = "Distortion";
            std::vector<krakendsp::FXControl> controls = {
                krakendsp::FXControl{"Gain", 1.0f, "", 0.0f, 1.0f, 0},
                krakendsp::FXControl{"Filter1 Freq", 10000.0f, "Hz", 10.0f, 20000.0f, 1},
                krakendsp::FXControl{"Filter1 Type", 0.0f, "", 0.0f, 5.0f, 2}
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

            float samplerateWithOversample() const {
                return samplerate * static_cast<float>(oversample);
            }

            void fill2xOversampleBufferMono(float input);
            void fill4xOversampleBufferMono();
            void fill8xOversampleBufferMono();
            void decimate4xOversampleBufferMono();
            void decimate2xOversampleBufferMono();

            void fill2xOversampleBufferStereo(Stereo input);
            void fill4xOversampleBufferStereo();
            void fill8xOversampleBufferStereo();
            void decimate4xOversampleBufferStereo();
            void decimate2xOversampleBufferStereo();

            const std::array<float, 47> aaFilterCoeffs = {
                -0.001126702003683413,
                0.000000000000000008,
                0.002205942120063343,
                -0.000000000000000004,
                -0.003778553259285692,
                0.000000000000000005,
                0.005980596978759721,
                -0.000000000000000007,
                -0.008991391999285685,
                0.000000000000000009,
                0.013070952693427127,
                -0.000000000000000011,
                -0.018641963114944413,
                0.000000000000000014,
                0.026484712056733570,
                -0.000000000000000016,
                -0.038269325401513309,
                0.000000000000000017,
                0.058366914019430158,
                -0.000000000000000018,
                -0.102902879550476875,
                0.000000000000000019,
                0.317427854911718654,
                0.500347685098113448,
                0.317427854911718654,
                0.000000000000000019,
                -0.102902879550476875,
                -0.000000000000000018,
                0.058366914019430158,
                0.000000000000000017,
                -0.038269325401513309,
                -0.000000000000000016,
                0.026484712056733570,
                0.000000000000000014,
                -0.018641963114944413,
                -0.000000000000000011,
                0.013070952693427127,
                0.000000000000000009,
                -0.008991391999285685,
                -0.000000000000000007,
                0.005980596978759721,
                0.000000000000000005,
                -0.003778553259285690,
                -0.000000000000000004,
                0.002205942120063343,
                0.000000000000000008,
                -0.001126702003683414
            };

            std::array<float, 256> aa2xInBufferL = {0.0f};
            size_t aa2xInBufferIndexL = 0;
            std::array<float, 256> aa4xInBufferL = {0.0f};
            size_t aa4xInBufferIndexL = 0;
            std::array<float, 256> aa8xInBufferL = {0.0f};
            size_t aa8xInBufferIndexL = 0;
            std::array<float, 256> aa2xOutBufferL = {0.0f};
            size_t aa2xOutBufferIndexL = 0;
            std::array<float, 256> aa4xOutBufferL = {0.0f};
            size_t aa4xOutBufferIndexL = 0;
            std::array<float, 256> aa8xOutBufferL = {0.0f};
            size_t aa8xOutBufferIndexL = 0;
            std::array<float, 256> aa2xInBufferR = {0.0f};
            size_t aa2xInBufferIndexR = 0;
            std::array<float, 256> aa4xInBufferR = {0.0f};
            size_t aa4xInBufferIndexR = 0;
            std::array<float, 256> aa8xInBufferR = {0.0f};
            size_t aa8xInBufferIndexR = 0;
            std::array<float, 256> aa2xOutBufferR = {0.0f};
            size_t aa2xOutBufferIndexR = 0;
            std::array<float, 256> aa4xOutBufferR = {0.0f};
            size_t aa4xOutBufferIndexR = 0;
            std::array<float, 256> aa8xOutBufferR = {0.0f};
            size_t aa8xOutBufferIndexR = 0;

            void applyFilterToBufferMono(bool inBuffer);
            void applyFilterToBufferStereo(bool inBuffer);

            void applyInterpolationTo2xInBuffer(float sample);
            void applyInterpolationTo4xInBuffer(float sample);
            void applyInterpolationTo8xInBuffer(float sample);
            void applyInterpolationTo2xInBuffer(Stereo sample);
            void applyInterpolationTo4xInBuffer(Stereo sample);
            void applyInterpolationTo8xInBuffer(Stereo sample);

            float previousSampleL = 0.0f;
            float previousSampleR = 0.0f;

            const int halfFilterSize = static_cast<int>(aaFilterCoeffs.size() / 2);

            bool interpolationUpsample = false;

            std::array<float, 8> upsampleBufferL = {0.0f};
            std::array<float, 8> upsampleBufferR = {0.0f};
            size_t upsampleBufferLength = 1;

            void fillUpSampleBuffer(float sample);
            void fillUpSampleBuffer(Stereo sample);
            float downsampleMono();
            Stereo downsampleStereo();

            BiquadFilter filter1;
    };
}

#endif // KDSP_DISTORTION_H_