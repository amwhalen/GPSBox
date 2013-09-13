/**
 * GPSBox by amwhalen
 */

#include "Arduino.h"
#include "GPSBox.h"

/**
 * Constructor
 * 
 * Required Digital Pins:
 * GPS TX
 * Servo
 * Button
 * LCD RS
 * LCD Enable
 * LCD D4
 * LCD D5
 * LCD D6
 * LCD D7
 * Pololu Power Switch
 */
GPSBox::GPSBox()
 : _gpsTxPin(6)
 , _servoPin(8)
 , _buttonPin(9)
 , _lcdRSPin(12)
 , _lcdEnablePin(11)
 , _lcdD4Pin(5)
 , _lcdD5Pin(4)
 , _lcdD6Pin(3)
 , _lcdD7Pin(2)
 , _powerPin(7)
 , _ss(255, _gpsTxPin)
 , _lcd(_lcdRSPin, _lcdEnablePin, _lcdD4Pin, _lcdD5Pin, _lcdD6Pin, _lcdD7Pin)
 , _targetLatitude(0)
 , _targetLongitude(0)
 , _targetDistance(500)
 , _maxAttempts(50)
 , _servoUnlockedPosition(20)
 , _servoLockedPosition(160)
 , _debug(false)
{

    // software serial input from GPS
    _ss.begin(4800);

    // init button
    pinMode(_buttonPin, INPUT);

    // init servo
    _servo.attach(_servoPin);

    // LCD
    _lcd.begin(16, 2);

    // pololu power switch
    pinMode(_powerPin, OUTPUT);

    // seed random generator
    randomSeed(analogRead(0));

}

uint8_t GPSBox::setGpsTxPin(uint8_t f) {
    _gpsTxPin = f;
    return f;
}

uint8_t GPSBox::setServoPin(uint8_t f) {
    _servoPin = f;
    return f;
}

uint8_t GPSBox::setButtonPin(uint8_t f) {
    _buttonPin = f;
    return f;
}

uint8_t GPSBox::setLcdRsPin(uint8_t f) {
    _lcdRSPin = f;
    return f;
}

uint8_t GPSBox::setLcdEnablePin(uint8_t f) {
    _lcdEnablePin = f;
    return f;
}

uint8_t GPSBox::setLcdD4Pin(uint8_t f) {
    _lcdD4Pin = f;
    return f;
}

uint8_t GPSBox::setLcdD5Pin(uint8_t f) {
    _lcdD5Pin = f;
    return f;
}

uint8_t GPSBox::setLcdD6Pin(uint8_t f) {
    _lcdD6Pin = f;
    return f;
}

uint8_t GPSBox::setLcdD7Pin(uint8_t f) {
    _lcdD7Pin = f;
    return f;
}

uint8_t GPSBox::setPowerPin(uint8_t f) {
    _powerPin = f;
    return f;
}

float GPSBox::setTargetLatitude(float l) {
    _targetLatitude = l;
    return l;
}

float GPSBox::setTargetLongitude(float l) {
    _targetLongitude = l;
    return l;
}

/**
 * Sets the distance in meters (radius) from the target location which will trigger the game to be solved.
 * 
 * @param uint16_t d The distance in meters. Must be in the 0-65535 range.
 * @return uint16_t The new distance.
 */
uint16_t GPSBox::setTargetDistance(uint16_t d) {
    _targetDistance = d;
    return d;
}

/**
 * Sets the position (in degrees) at which your servo lock is "unlocked".
 *
 * @param uint8_t pos The position (in degrees) at which your servo lock is "unlocked". Usually in the 0-180 range.
 * @return uint8_t The new unlocked position.
 */
uint8_t GPSBox::setServoUnlockedPosition(uint8_t pos) {
    _servoUnlockedPosition = pos;
    return pos;
}

/**
 * Sets the position (in degrees) at which your servo lock is "locked".
 *
 * @param uint8_t pos The position (in degrees) at which your servo lock is "locked". Usually in the 0-180 range.
 * @return uint8_t The new locked position.
 */
uint8_t GPSBox::setServoLockedPosition(uint8_t pos) {
    _servoLockedPosition = pos;
    return pos;
}

/**
 * Sets the maximum number of attempts allowed. Must be in the 0-255 range.
 * 
 * @param  uint8_t a The number of attempts.
 * @return uint8_t The current number of attempts.
 */
uint8_t GPSBox::setMaxAttempts(uint8_t a) {
    _maxAttempts = a;
    return a;
}

/**
 * Turns on debug mode, printing to Serial at 9600bps.
 *
 * @return void
 */
void GPSBox::debug() {
    _debug = true;
    Serial.begin(9600);
}

/**
 * Runs the GPS Box game. Call this after setting all the correct pins and game parameters.
 * 
 * @return void
 */
void GPSBox::run() {
    
    // Power is turned on (the button was pressed and pololu turned the power on, or power was applied to the external barrel jack)

    if (isSolved()) {

        win();

    } else {

        // (re)close the locking servo
        servoLock();

        // If ATTEMPTS_REMAINING > 0:
        if (getAttemptsRemaining() > 0) {

            // set EEPROM ATTEMPTS_REMAINING = ATTEMPTS_REMAINING - 1
            setAttemptsRemaining(getAttemptsRemaining() - 1);

            // display "Attempts Remain: / NN of 50"
            String attemptString = String(getAttemptsRemaining());
            String maxAttemptString = String(getMaxAttempts());
            displayMessage("Attempts Remain:", attemptString + " of " + maxAttemptString);

            // delay 5 seconds
            delay(5000);

            // turn on GPS and search for signal
            displayMessage("Searching...", "");
            bool foundSignal = waitForGPSSignal(60000);
            if (foundSignal) {
                
                // calculate the distance from the current location to the target location
                float flat, flng;
                unsigned long age;
                _gps.f_get_position(&flat, &flng, &age);
                float distance = _gps.distance_between(flat, flng, _targetLatitude, _targetLongitude);

                // If the target location is within the minimum distance:
                if (distance <= _targetDistance) {
                    
                    win();

                } else {
                    
                    // ACCESS DENIED
                    displayMessage("Access Denied!", "");
                    delay(3000);

                    // give distance to target
                    int distanceInWholeMeters = (int) distance;
                    String distanceString = String(distanceInWholeMeters);
                    displayMessage("Distance: ", distanceString + " meters");
                    delay(10000);

                }

            } else {
                displayMessage("No GPS Signal", "");
                delay(10000);
            }                
                
        } else {
            
            // no attempts remain!!
            for (uint8_t i = 0; i < 3; i++) {
                displayMessage("Game Over", "Locked Forever");
                delay(5000);
                displayMessage("Return To", "Manufacturer");
                delay(5000);
            }

        }

    }        

    // SHUTDOWN:
    for (uint8_t i = 5; i > 0; i--) {
        String secondString = String(i);
        displayMessage("Powering off in:", secondString+"...");
        delay(1000);
    }
    // TODO: turn off power by sending a HIGH signal to the pololu switch

    /**
     * DEVELOPER ACCESS
     *
     * If the device is powered through the barrel jack (and not the pololu),
     * the arduino will run the code below here.
     */

    // allow back door access for the developer (or smart players)
    backdoor();

    // delay 20 seconds so we have a chance to keep the bxo in this state
    displayMessage("Remove Power", "To Keep Unlocked");
    delay(20000);

    // lock the box, ready it for a game
    reset();

}

/**
 * PRIVATE
 */

/**
 * Performs actions needed to put the game in a solved state.
 *
 * @return void
 */
void GPSBox::win() {
    displayMessage("Congratulations!", "");
    setSolved();
    delay(5000);
}

/**
 * Secret backdoor code. Device must be powered through the barrel jack, overriding the pololu power switching.
 *
 * @return void
 */
void GPSBox::backdoor() {

    displayMessage("Excess voltage!", "Remove Power!");
    delay(10000);
    
    // display "garbage" characters to simulate an error
    //displayMessage("Garbage", "Characters");
    unsigned long garbageTimeout;
    if (isDebugging()) {
        garbageTimeout = 10000;
    } else {
        garbageTimeout = 120000;
    }
    displayGarbage(garbageTimeout);  
    
    // count down to open
    for (uint8_t i = 5; i > 0; i--) {
        String openInString = String(i);
        displayMessage("Box opens in:", openInString + "...");
        delay(1000);
    }

    // open the box by placing the servo in the OPENED position
    servoUnlock();

    // set SOLVED in EEPROM
    setSolved();

}

/**
 * Resets the state of the box so it can be played from the beginning.
 *
 * @return void
 */
void GPSBox::reset() {

    // count down to lock
    for (uint8_t i = 5; i > 0; i--) {
        String lockInString = String(i);
        displayMessage("Locking in:", lockInString + "...");
        delay(1000);
    }

    // LOCK:
    // set the servo to the LOCKED position
    servoLock();
    
    // set ATTEMPTS_REMAINING to maximum amount in EEPROM
    String attemptString = String(getMaxAttempts());
    displayMessage("Max Attempts", attemptString);
    setAttemptsRemaining(getMaxAttempts());
    delay(5000);
    
    // set SOLVED to 0 in EEPROM
    setUnsolved();

    // display "Locked"
    displayMessage("LOCKED", "Game reset");

}

/**
 * Waits for the GPS to get a valid signal.
 * 
 * @param  int timeout The number of milliseconds to wait and try to get a signal.
 * @return bool Returns true if a valid signal was found within the timeout, false if not.
 */
bool GPSBox::waitForGPSSignal(unsigned long timeout) {
    // timeout in milliseconds
    unsigned long startTime = millis();
    while (millis() - startTime < timeout) {
        while (_ss.available()) {
            // read another character and see if it completes a NMEA sentence
            if (_gps.encode(_ss.read())) {
                // _gps is now populated with valid data
                return true;
            }
            // update display with a progress bar based on timeout?
        }
    }
    return false;
}

/**
 * Place the servo in its "locked" position
 *
 * @return void
 */
void GPSBox::servoLock() {
    _servo.write(_servoLockedPosition);
    delay(500);
}

/**
 * Place the servo in its "unlocked" position
 *
 * @return void
 */
void GPSBox::servoUnlock() {
    _servo.write(_servoUnlockedPosition);
    delay(500);
}

/**
 * Turn the power to the Pololu device off
 *
 * @return void
 */
void GPSBox::powerOff() {
    digitalWrite(_powerPin, HIGH);
}

/**
 * Returns if the game has been solved.
 * 
 * @return bool Returns true if the game has been solved, false otherwise.
 */
bool GPSBox::isSolved() {
    return (EEPROM.read(0) == 1);
}

/**
 * Returns the number of attempts remaining to solve the game.
 * 
 * @return uint8_t Number of attempts remaining.
 */
uint8_t GPSBox::getAttemptsRemaining() {
    return EEPROM.read(1);
}

/**
 * Sets the number of attempts remaining.
 * 
 * @param uint8_t a The new number of attempts remaining.
 * @return uint8_t The current number of attempts remaining.
 */
uint8_t GPSBox::setAttemptsRemaining(uint8_t a) {
    EEPROM.write(1, a);
    return a;
}

/**
 * Sets the game as solved.
 * 
 * @return void
 */
void GPSBox::setSolved() {
    EEPROM.write(0, 1);
}

/**
 * Sets the game as unsolved.
 * 
 * @return void
 */
void GPSBox::setUnsolved() {
    EEPROM.write(0, 0);
}

/**
 * Returns the maximum number of attempts to solve the game.
 * 
 * @return uint8_t The maximum number of attempts.
 */
uint8_t GPSBox::getMaxAttempts() {
    return _maxAttempts;
}

/**
 * Returns true if debug mode is on.
 * 
 * @return bool True if debug mode is on, false otherwise.
 */
bool GPSBox::isDebugging() {
    return _debug;
}

/**
 * Displays a message on the 16x2 LCD.
 * 
 * @param String line1 The string to display on line 1.
 * @param String line2 The string to display on line 2.
 */
void GPSBox::displayMessage(String line1, String line2) {
    
    if (isDebugging()) {
        Serial.println("================");
        Serial.println(line1);
        Serial.println(line2);
        Serial.println("================");
        Serial.println();
    }

    _lcd.setCursor(0, 0);
    _lcd.print(line1);
    _lcd.setCursor(0, 1);
    _lcd.print(line2);

}

/**
 * Displays garbage on the LCD for the given milliseconds
 * @param timeout [description]
 */
void GPSBox::displayGarbage(unsigned long timeout) {

    // special characters
    // http://www.stanford.edu/class/ee281/handouts/lcd_tutorial.pdf

    int x, y, index;
    char garbageCharacters[] = {0xA6, 0xFF, 0xD0, 0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF, 0xB1, 0xB2, 0xB3, 0xB5, 0xB6, 0xB7, 0xB8};
    char garbage;

    unsigned long startTime = millis();
    while (millis() - startTime < timeout) {
        if (isDebugging()) {
            Serial.println("================");
        }
        for (y = 0; y < 2; y++) {
            for (x = 0; x < 16; x++) {
                index = random(sizeof(garbageCharacters));
                _lcd.setCursor(x, y);
                _lcd.write(garbageCharacters[index]);
                if (isDebugging()) {
                    Serial.print(garbageCharacters[index]);
                }
            }
            if (isDebugging()) {
                Serial.println();
            }
        }
        if (isDebugging()) {
            Serial.println("================");
            Serial.println();
        }
        delay(500 + random(1000));
    }
}

