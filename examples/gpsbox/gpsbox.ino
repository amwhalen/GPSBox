#include <GPSBox.h>

// TinyGPS Library: http://arduiniana.org/libraries/tinygps/
#include "TinyGPS.h"

// Standard Arduino Libraries
#include "SoftwareSerial.h"
#include "LiquidCrystal.h"
#include "Servo.h"
#include "EEPROM.h"

void setup() {
  
    GPSBox g;

    // hardware pins
    g.setGpsTxPin(6);
    g.setServoPin(8);
    g.setButtonPin(9);
    g.setLcdRsPin(12);
    g.setLcdEnablePin(11);
    g.setLcdD4Pin(5);
    g.setLcdD5Pin(4);
    g.setLcdD6Pin(3);
    g.setLcdD7Pin(2);

    // game parameters
    g.setTargetLatitude(42.388427);
    g.setTargetLongitude(-72.526645);
    g.setTargetDistance(500); // meters

    g.run();

}

void loop() {
    // nada
}
