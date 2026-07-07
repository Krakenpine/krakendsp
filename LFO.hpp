#ifndef KDSP_LFO_H_
#define KDSP_LFO_H_

namespace krakendsp
{

enum class LFOType {
    SINE,
    SQUARE,
    TRIANGLE,
    SAWTOOTH
};

class LFO
{
    public:
        LFO() = default;
        LFO(float frequency_, float sampleRate_);
        LFO(float frequency_, float sampleRate_, size_t sinTableSize_, float* sinTable_);
        ~LFO() = default;

        LFOType getType() const;

        float getFrequency() const;
        float getNextSample();
        float getNextSampleUnipolar();
        float getHorizontalDistort() const;
        float getVerticalDistort() const;
        float getFilterFreq() const;
        float getSampleRate() const;

        void setType(LFOType type_);
        void setFrequency(float frequency_);
        void setVerticalDistort(float verticalDistort_);
        void setHorizontalDistort(float horizontalDistort_);
        void setSinTable (const float* table, size_t size);
        void setFilter(float filtFreq_);
        void setSampleRate(float sampleRate_);

    private:
        float frequency = 1.0f;
        float phase = 0.0f;
        float sampleRate = 48000.0f;
        float phaseIncrement = frequency / sampleRate;
        float verticalDistort = 0.5f;
        float horizontalDistort = 0.5f;
        LFOType type = LFOType::SINE;
        float* sinTable = nullptr;
        size_t sinTableSize = 0;

        float getSin(float phase_);
        float prevSample = 0.0f;
        float filterFreq = 0.0f;
        float filterFreqInv = 1.0f;

};

}
#endif // KDSP_LFO_H_