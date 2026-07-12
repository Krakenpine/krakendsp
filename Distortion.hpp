#ifndef KDSP_DISTORTION_H_
#define KDSP_DISTORTION_H_

#include "MultiFX.hpp"
#include <vector>
#include <string>
#include <array>

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
            
        private:
            float gain = 0;
            float samplerate = 44100;
            Oversample oversample = Oversample::None;
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

            float samplerateWithOversample() const {
                return samplerate * static_cast<float>(oversample);
            }

            std::array<float, 2> oversample2xBuffer = {0.0f, 0.0f};
            std::array<float, 4> oversample4xBuffer = {0.0f, 0.0f, 0.0f, 0.0f};
            std::array<float, 8> oversample8xBuffer = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

            void fill2xOversampleBuffer(float input);
            void fill4xOversampleBuffer();
            void fill8xOversampleBuffer();
            void decimate4xOversampleBuffer();
            void decimate2xOversampleBuffer();

            float prevValue = 0;
            float prevOutput = 0;
            float prevFiltInput = 0;

            float prevFilt1Input = 0;
            float prevFilt1Output = 0;
            float prevFilt2Input = 0;
            float prevFilt2Output = 0;

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

            std::array<float, 256> aa2xInBuffer = {0.0f};
            size_t aa2xInBufferIndex = 0;
            std::array<float, 256> aa4xInBuffer = {0.0f};
            size_t aa4xInBufferIndex = 0;
            std::array<float, 256> aa8xInBuffer = {0.0f};
            size_t aa8xInBufferIndex = 0;
            std::array<float, 256> aa2xOutBuffer = {0.0f};
            size_t aa2xOutBufferIndex = 0;
            std::array<float, 256> aa4xOutBuffer = {0.0f};
            size_t aa4xOutBufferIndex = 0;
            std::array<float, 256> aa8xOutBuffer = {0.0f};
            size_t aa8xOutBufferIndex = 0;

            void applyFilterToBuffer(bool inBuffer);

            void applyInterpolationTo2xInBuffer(float sample);
            void applyInterpolationTo4xInBuffer(float sample);
            void applyInterpolationTo8xInBuffer(float sample);

            float previousSample = 0.0f;

            const int halfFilterSize = static_cast<int>(aaFilterCoeffs.size() / 2);

            bool interpolationUpsample = false;

            std::array<float, 8> upsampleBuffer = {0.0f};
            size_t upsampleBufferLength = 1;

            void fillUpSampleBuffer(float sample);
            float downsample();
    };
}

#endif // KDSP_DISTORTION_H_