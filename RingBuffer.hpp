#ifndef KDSP_RINGBUFFER_H_
#define KDSP_RINGBUFFER_H_

#include <vector>

namespace krakendsp
{
    class RingBuffer
    {
        public:
            RingBuffer();
            RingBuffer(size_t size);
            RingBuffer(std::vector<float>* buffer_);
            ~RingBuffer() = default;

            float getNextSample();
            void writeNextSample(float sample);
            float getNextAndWriteSample(float newSample);
            void setLength(float newLength);
            void setLengthTarget(float newLength);

            float getLength() const;

            void reset();

        private:
            std::vector<float> buffer;
            std::vector<float>* bufferPtr;
            size_t bufferSize;
            size_t writeHead;
            size_t readHead;
            float readHeadDecimal;
            float delayLength;
            float delayLengthTarget;

            void adjustDelayLength();
    };
}

#endif // KDSP_RINGBUFFER_H_