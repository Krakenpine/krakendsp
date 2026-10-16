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

            bool getInterpolationUpsampleMode() const;

            FileType getFileType() const override;
            std::vector<AcceptedFile> getAcceptedFiles() const override;
            void setFile(const FXFile& file, size_t fileIndex) override;

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
                krakendsp::FXControl{
                    .name = "Gain",
                    .displayMultiplier = 1.0f,
                    .unit = "", 
                    .valueMin = 0.0f,
                    .valueMax = 1.0f,
                    .index = 0
                },
                krakendsp::FXControl{
                    .name = "Filter1 Freq",
                    .displayMultiplier = 1.0f,
                    .unit = "Hz",
                    .valueMin = 10.0f,
                    .valueMax = 20000.0f,
                    .index = 1
                },
                krakendsp::FXControl{
                    .name = "Filter1 Type",
                    .displayMultiplier = 1.0f,
                    .unit = "",
                    .valueMin = 0.0f,
                    .valueMax = 5.0f,
                    .index = 2
                }
            };

            std::vector<krakendsp::FXSubType> types = {
                krakendsp::FXSubType{.name = "Raw", .typeID = 0},
                krakendsp::FXSubType{.name = "Softclip", .typeID = 1},
                krakendsp::FXSubType{.name = "Hardclip", .typeID = 2},
                krakendsp::FXSubType{.name = "MetalZone", .typeID = 3},
                krakendsp::FXSubType{.name = "Wavefolder", .typeID = 4},
                krakendsp::FXSubType{.name = "Clean", .typeID = 5}
            };

            float processRaw(float input) const;
            float processClean(float input) const;
            float processSoftClip(float input) const;
            float processHardClip(float input) const;
            float processWavefolder(float input) const;

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

            std::vector<float> aaFilterCoeffsVector = {
                -0.001126702003683413f,
                0.000000000000000008f,
                0.002205942120063343f,
                -0.000000000000000004f,
                -0.003778553259285692f,
                0.000000000000000005f,
                0.005980596978759721f,
                -0.000000000000000007f,
                -0.008991391999285685f,
                0.000000000000000009f,
                0.013070952693427127f,
                -0.000000000000000011f,
                -0.018641963114944413f,
                0.000000000000000014f,
                0.026484712056733570f,
                -0.000000000000000016f,
                -0.038269325401513309f,
                0.000000000000000017f,
                0.058366914019430158f,
                -0.000000000000000018f,
                -0.102902879550476875f,
                0.000000000000000019f,
                0.317427854911718654f,
                0.500347685098113448f,
                0.317427854911718654f,
                0.000000000000000019f,
                -0.102902879550476875f,
                -0.000000000000000018f,
                0.058366914019430158f,
                0.000000000000000017f,
                -0.038269325401513309f,
                -0.000000000000000016f,
                0.026484712056733570f,
                0.000000000000000014f,
                -0.018641963114944413f,
                -0.000000000000000011f,
                0.013070952693427127f,
                0.000000000000000009f,
                -0.008991391999285685f,
                -0.000000000000000007f,
                0.005980596978759721f,
                0.000000000000000005f,
                -0.003778553259285690f,
                -0.000000000000000004f,
                0.002205942120063343f,
                0.000000000000000008f,
                -0.001126702003683414f
            };
            
            std::vector<krakendsp::Stereo> aa2xInBufferVector = std::vector<krakendsp::Stereo>(256);
            size_t aa2xInBufferIndexVector = 0;
            std::vector<krakendsp::Stereo> aa4xInBufferVector = std::vector<krakendsp::Stereo>(256);
            size_t aa4xInBufferIndexVector = 0;
            std::vector<krakendsp::Stereo> aa8xInBufferVector = std::vector<krakendsp::Stereo>(256);
            size_t aa8xInBufferIndexVector = 0;
            std::vector<krakendsp::Stereo> aa2xOutBufferVector = std::vector<krakendsp::Stereo>(256);
            size_t aa2xOutBufferIndexVector = 0;
            std::vector<krakendsp::Stereo> aa4xOutBufferVector = std::vector<krakendsp::Stereo>(256);
            size_t aa4xOutBufferIndexVector = 0;
            std::vector<krakendsp::Stereo> aa8xOutBufferVector = std::vector<krakendsp::Stereo>(256);
            size_t aa8xOutBufferIndexVector = 0;

            void applyFilterToBufferStereoWithVectors(bool inBuffer, bool isMono);

            void applyInterpolationTo2xInBuffer(Stereo sample);
            void applyInterpolationTo4xInBuffer(Stereo sample);
            void applyInterpolationTo8xInBuffer(Stereo sample);

            float previousSampleL = 0.0f;
            float previousSampleR = 0.0f;

            krakendsp::Stereo previousSample = {0.0f, 0.0f};

            unsigned int halfFilterSize = static_cast<unsigned int>(aaFilterCoeffsVector.size() / 2);

            bool interpolationUpsample = false;

            std::vector<krakendsp::Stereo> upsampleBufferVector = std::vector<krakendsp::Stereo>(8);
            size_t upsampleBufferLength = 1;

            void fillUpSampleBuffer(float sample);
            void fillUpSampleBuffer(Stereo sample);
            float downsampleMono();
            Stereo downsampleStereo();

            BiquadFilter filter1;
    };
}

#endif // KDSP_DISTORTION_H_