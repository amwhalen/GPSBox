GPSBox
======

Arduino library for creating a location-aware puzzle box. The box only opens when near the correct coordinates.

Required Hardware
-----------------

* Arduino
* GPS unit
* 16x2 LCD
* Momentary Switch
* Servo (with custom locking mechanism)
* Box

Required Software
-----------------

The [TinyGPS Library](http://arduiniana.org/libraries/tinygps/) is required for parsing the GPS data.
All of the other libraries are part of the standard Arduino libraries and should already exist on your system.

Installation
------------

Add this library to your standard Arduino library directory location.
See the [Arduino Library](http://arduino.cc/en/Guide/Libraries) documentation for more help on the topic.

Example Usage
-------------

    #include <GPSBox.h>

    #include "TinyGPS.h"
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

License
-------

GPSBox is released under the terms of the [MIT License](http://www.opensource.org/licenses/mit-license.html).

The MIT License (MIT)
Copyright (c) 2013 Andrew M. Whalen

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.