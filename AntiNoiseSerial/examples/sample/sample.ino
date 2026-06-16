#include <AntiNoiseSerial.h>

// Initialization: Using hardware Serial, 128 bytes buffer size, Spacing Mode enabled (true)
// Set the third parameter to 'false' if you want characters to merge tightly without spaces when noise occurs.
AntiNoiseSerial antiNoise(Serial, 128, true);

void setup() {
    Serial.begin(9600);
    while (!Serial) { ; } // Wait for serial monitor to connect
    
    Serial.println(F("=============================================="));
    Serial.println(F("    Anti Noise Serial Library Demo Loaded    "));
    Serial.println(F("=============================================="));
    Serial.println(F("Type something into the Serial Monitor!"));
    Serial.println(F("Try patterns like: TURN_ON_LED or SET_TEMP:25"));
    Serial.println(F("----------------------------------------------"));
}

void loop() {
    // Constantly call update() to process incoming stream data
    antiNoise.update();

    String currentBuffer = antiNoise.getBuffer();

    // Check if the user pressed Enter (detected by newline character '\n')
    if (currentBuffer.indexOf('\n') != -1) {
        
        Serial.print(F("[CLEAN BUFFER]: "));
        Serial.print(currentBuffer);

        // --- PATTERN EVALUATION USING LIKE FUNCTION ---

        // 1. Matches if the word "ON" exists anywhere in the buffer (Like %ON%)
        if (antiNoise.like("%ON%")) {
            Serial.println(F("[ACTION] 'ON' Command Detected!"));
        }

        // 2. Matches "SET_TEMP:" followed by any dynamic values (Like %SET_TEMP:%%)
        else if (antiNoise.like("%SET_TEMP:%")) {
            Serial.println(F("[ACTION] Temperature Configuration Received!"));
        }

        // 3. Strict position match using single wildcard '_' (e.g., ID01, IDAB)
        else if (antiNoise.like("%ID__%")) {
            Serial.println(F("[ACTION] Valid 2-Digit ID Detected!"));
        }
        
        else {
            Serial.println(F("[INFO] Pattern did not match any LIKE filter rules."));
        }

        // Always clear the buffer after processing to receive new commands
        antiNoise.clear();
        Serial.println(F("----------------------------------------------"));
    }
}