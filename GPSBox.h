/**
 * GPSBox by amwhalen
 */

#ifndef GPSBox_h
#define GPSBox_h

#include "Arduino.h"

// also include these libraries in your sketch:
#include "TinyGPS.h"
#include "SoftwareSerial.h"
#include "LiquidCrystal.h"
#include "Servo.h"
#include "EEPROM.h"

class GPSBox {
    public:
        GPSBox();
        void run();
        uint8_t setGpsTxPin(uint8_t);
        uint8_t setServoPin(uint8_t);
        uint8_t setButtonPin(uint8_t);
        uint8_t setLcdRsPin(uint8_t);
        uint8_t setLcdEnablePin(uint8_t);
        uint8_t setLcdD4Pin(uint8_t);
        uint8_t setLcdD5Pin(uint8_t);
        uint8_t setLcdD6Pin(uint8_t);
        uint8_t setLcdD7Pin(uint8_t);
        uint8_t setPowerPin(uint8_t);
        uint8_t setServoUnlockedPosition(uint8_t);
        uint8_t setServoLockedPosition(uint8_t);
        uint8_t setMaxAttempts(uint8_t);
        float setTargetLatitude(float);
        float setTargetLongitude(float);
        uint16_t setTargetDistance(uint16_t);
        void debug();

    private:
        Servo _servo;
        TinyGPS _gps;
        SoftwareSerial _ss;
        LiquidCrystal _lcd;
        uint8_t _gpsTxPin;
        uint8_t _servoPin;
        uint8_t _buttonPin;
        uint8_t _lcdRSPin;
        uint8_t _lcdEnablePin;
        uint8_t _lcdD4Pin;
        uint8_t _lcdD5Pin;
        uint8_t _lcdD6Pin;
        uint8_t _lcdD7Pin;
        uint8_t _powerPin;
        uint8_t _servoUnlockedPosition;
        uint8_t _servoLockedPosition;
        uint8_t _maxAttempts;
        float _targetLatitude;
        float _targetLongitude;
        uint16_t _targetDistance;
        bool _debug;
        void displayMessage(String, String);
        void displayGarbage(unsigned long);
        bool waitForGPSSignal(unsigned long);
        void servoLock();
        void servoUnlock();
        bool isSolved();
        void setSolved();
        void setUnsolved();
        uint8_t getMaxAttempts();
        uint8_t getAttemptsRemaining();
        uint8_t setAttemptsRemaining(uint8_t);
        void win();
        void backdoor();
        void reset();
        void powerOff();
        bool isDebugging();
};

#endif