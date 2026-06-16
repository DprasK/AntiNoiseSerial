#ifndef ANTI_NOISE_SERIAL_H
#define ANTI_NOISE_SERIAL_H

#include <Arduino.h>

class AntiNoiseSerial {
private:
    Stream* _serial;
    String _buffer;
    size_t _maxBufferSize;
    bool _useSpacing;
    bool _lastCharWasNoise;

    // Internal recursive method for MySQL LIKE pattern matching
    bool matchLike(const char* str, const char* pattern);

public:
    // Constructor (Default: max buffer size 128 bytes, spacing mode disabled)
    AntiNoiseSerial(Stream& serialPort, size_t maxBufSize = 128, bool useSpacing = false);

    // Reads data from the serial port and filters out electrical noise
    void update();

    // Returns the current clean string buffer
    String getBuffer();

    // Clears the internal string buffer
    void clear();

    // Pattern matching feature similar to MySQL LIKE (Supports '%' and '_')
    bool like(String pattern);
};

#endif