#include "Ringbuffer.hpp"
#include <vector>
#include <iostream>
#include <cmath>
#include <algorithm>

krakendsp::RingBuffer::RingBuffer() : 
    bufferSize(1),
    bufferPtr(&buffer),
    writeHead(0),
    readHead(0),
    readHeadDecimal(0.0f),
    delayLength(1),
    delayLengthTarget(1) { }

krakendsp::RingBuffer::RingBuffer(size_t size) :
    bufferSize(size),
    bufferPtr(&buffer),
    writeHead(0),
    readHead(0),
    readHeadDecimal(0.0f),
    delayLength(static_cast<float>(size) - 1),
    delayLengthTarget(static_cast<float>(size) - 1) {
        buffer.resize(size);
    }

krakendsp::RingBuffer::RingBuffer(std::vector<float>* buffer_) :
    bufferSize(buffer_->size()),
    bufferPtr(buffer_),
    writeHead(0),
    readHead(0),
    readHeadDecimal(0.0f),
    delayLength(static_cast<float>(buffer_->size()) - 1),
    delayLengthTarget(static_cast<float>(buffer_->size()) - 1) { }

float krakendsp::RingBuffer::getNextSample() {
    adjustDelayLength();
    size_t lengthWhole = static_cast<size_t>(floor(delayLength));
    float readHeadDecimal = delayLength - static_cast<float>(lengthWhole);

    size_t readIndex = (writeHead - lengthWhole + bufferSize) % bufferSize;
    size_t prevReadIndex = (readIndex - 1 + bufferSize) % bufferSize;

    float sample = 0.0f;

    sample = buffer[readIndex] * (1.0f - readHeadDecimal) + readHeadDecimal * buffer[prevReadIndex];
    return sample;
}

void krakendsp::RingBuffer::writeNextSample(float sample) {
    if (writeHead < bufferSize) {
        buffer[writeHead] = sample;
    }
    
    writeHead = (writeHead + 1) % bufferSize;
}

float krakendsp::RingBuffer::getNextAndWriteSample(float newSample) {
    float sample = getNextSample();
    writeNextSample(newSample);
    return sample;
}

void krakendsp::RingBuffer::setLength(float newLength) {
    auto bufferSizeFloat = static_cast<float>(bufferSize);
    delayLength = std::clamp(newLength, 1.0f, bufferSizeFloat);
    delayLengthTarget = delayLength;
}

void krakendsp::RingBuffer::setLengthTarget(float newLength) {
    delayLengthTarget = std::clamp(newLength, 1.0f, static_cast<float>(bufferSize));
}

void krakendsp::RingBuffer::adjustDelayLength() {
    if (delayLength < delayLengthTarget) {
        if (delayLength + 0.5f > delayLengthTarget) {
            delayLength = delayLengthTarget;
        } else {
            delayLength += 0.5f; // Slowly increase the delay length
        }
    } else if (delayLength > delayLengthTarget) {
        if (delayLength - 0.5f < delayLengthTarget) {
            delayLength = delayLengthTarget;
        } else {
            delayLength -= 0.5f; // Slowly decrease the delay length
        }
    }
}

float krakendsp::RingBuffer::getLength() const {
    return delayLength;
}