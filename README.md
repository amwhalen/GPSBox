GPSBox
======

Arduino library for creating a location-aware puzzle box.
A box that only opens when near the correct coordinates.

The device is a locked box with only a LCD character screen and a button visible on the outside.
When the player presses the button, a GPS signal is acquired, and one of two things will happen:
* The distance to the target is shown on screen, then the device powers itself off.
* Or, if the box is within the specified range of the coordinates, the box will unlock.

The locking mechanism relies on a servo to move some sort of latch to lock and unlock the box.
A Pololu power switch allows the Arduino to shut itself off when not in use to conserve battery power.

This project was heavily inspired by [The Reverse Geocache Puzzle](http://arduiniana.org/projects/the-reverse-geo-cache-puzzle/).
See that project for ideas on building the hardware and hooking it all up.

Required Hardware
-----------------

* Arduino
* GPS module
* 16x2 LCD
* Momentary Switch
* Servo (with custom locking mechanism)
* Pololu power switch
* 4xAA battery holder
* Box

Required Software
-----------------

The [TinyGPS library](http://arduiniana.org/libraries/tinygps/) is required for parsing the GPS data.
All of the other libraries are part of the standard Arduino libraries and should already exist on your system.

Installation
------------

Add GPSBox and [TinyGPS](http://arduiniana.org/libraries/tinygps/) to your standard Arduino library directory location.
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

Unlocking and Locking the Box
-----------------------------

A developer back door is built in so you can unlock the box, or optionally reset and lock it.
By plugging power directly into the barrel jack, or through USB, you can access the back door by waiting a few minutes after the "Powering Off" message.
After about two minutes, after the fake power overload, you'll see some message on the screen that indicate the box is opened.
Once at this point, if you'd like to leave the box open, just remove the power source.
If you want to reset and lock the box, wait a few more seconds, and it will all be reset for a new game.

Debugging
---------

If you don't have an LCD hooked up yet, you can enable debugging to see the game messages on your computer.
Hook your project up through USB, upload the sketch, and watch the 9600bps output in your serial monitor.
Debug mode also reduces the amount of time needed to wait for the back door access.

    GPSBox g;
    g.debug();

Servo Positions
---------------

Some servos don't go from 0-180 degrees, and may operate strangely if forced to positions outside their specifications.
You can set the lock and unlock position (in degrees) for your specific servo using these functions:

    GPSBox g;
    g.setServoUnlockedPosition(20);
    g.setServoLockedPosition(160);

Game Parameters
---------------

There are four game parameters you can set using the following functions:

    GPSBox g;
    g.setTargetLatitude(42.388427);
    g.setTargetLongitude(-72.526645);
    g.setTargetDistance(500); // In meters
    g.setMaxAttempts(50); // The default is 50, but you can give more or less

License
-------

GPSBox is released under the terms of the [MIT License](http://www.opensource.org/licenses/mit-license.html).

The MIT License (MIT)
Copyright (c) 2013 Andrew M. Whalen

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.