#include "AntiNoiseSerial.h"

AntiNoiseSerial::AntiNoiseSerial(Stream& serialPort, size_t maxBufSize, bool useSpacing) {
    _serial = &serialPort;
    _maxBufferSize = maxBufSize;
    _useSpacing = useSpacing;
    _lastCharWasNoise = false;
    _buffer = "";
}

void AntiNoiseSerial::update() {
    while (_serial->available() > 0) {
        char c = _serial->read();

        // Filter ASCII: Allow readable characters (32 - 126), Carriage Return (\r), and New Line (\n)
        if ((c >= 32 && c <= 126) || c == '\r' || c == '\n') {
            if (_buffer.length() < _maxBufferSize) {
                _buffer += c;
            } else {
                // Prevent buffer overflow using FIFO style approach
                _buffer.remove(0, 1);
                _buffer += c;
            }
            _lastCharWasNoise = false;
        } 
        // Handle unreadable characters (Noise)
        else {
            if (_useSpacing) {
                // Insert a single space if the previous character was valid text
                if (!_lastCharWasNoise && _buffer.length() > 0) {
                    if (_buffer.charAt(_buffer.length() - 1) != ' ') {
                        if (_buffer.length() < _maxBufferSize) {
                            _buffer += ' ';
                        }
                    }
                }
            }
            _lastCharWasNoise = true;
        }
    }
}

String AntiNoiseSerial::getBuffer() { return _buffer; }
void AntiNoiseSerial::clear() { _buffer = ""; }
bool AntiNoiseSerial::like(String pattern) { return matchLike(_buffer.c_str(), pattern.c_str()); }

// Recursive Pattern Matching Algorithm (MySQL LIKE Style)
bool AntiNoiseSerial::matchLike(const char* str, const char* pattern) {
    if (*pattern == '\0') return *str == '\0';
    
    // Handle '%' wildcard (matches 0 or more characters)
    if (*pattern == '%') {
        while (*pattern == '%') pattern++; // Skip consecutive '%' wildcards
        if (*pattern == '\0') return true;
        while (*str != '\0') {
            if (matchLike(str, pattern)) return true;
            str++;
        }
        return false;
    }
    
    // Handle '_' wildcard (matches exactly 1 character)
    if (*pattern == '_') {
        if (*str == '\0') return false;
        return matchLike(str + 1, pattern + 1);
    }
    
    // Strict match for standard characters
    if (*str == *pattern) return matchLike(str + 1, pattern + 1);
    
    return false;
}