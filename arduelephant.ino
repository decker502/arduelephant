
#include <Wire.h>
#include <SparkFun_APDS9960.h>

#include "elephant.h"

// sensor 3 对应的引脚
#define PlayerPin 49
#define PlayerBusyPin 68

Elephant elephant = Elephant(PlayerPin, PlayerBusyPin);
SparkFun_APDS9960 apds = SparkFun_APDS9960();

void setup()
{
    // Be sure to set your serial console to 115200 baud
    Serial.begin(115200);

    elephant.begin();

    // Initialize APDS-9960 (configure I2C and initial values)
    if (apds.init())
    {
        Serial.println(F("APDS-9960 initialization complete"));
    }
    else
    {
        Serial.println(F("Something went wrong during APDS-9960 init!"));
        return;
    }

    // Start running the APDS-9960 gesture sensor engine
    if (apds.enableGestureSensor(true))
    {
        Serial.println(F("Gesture sensor is now running"));
    }
    else
    {
        Serial.println(F("Something went wrong during gesture sensor init!"));
        return;
    }
}

void loop()
{
    delay(100);

    if (apds.isGestureAvailable())
    {
        switch (apds.readGesture())
        {
        case DIR_UP:
            Serial.println("UP");
            elephant.grab();
            Serial.println("grab");

            break;
        case DIR_DOWN:
            Serial.println("DOWN");
            elephant.lift();
            elephant.roar();
            Serial.println("lift and roar");

            break;
        case DIR_LEFT:
            Serial.println("LEFT");
            elephant.forward();

            break;
        case DIR_RIGHT:
            Serial.println("RIGHT");
            elephant.backward();

            break;
        case DIR_NEAR:
            Serial.println("NEAR");
            break;
        case DIR_FAR:
            Serial.println("FAR");
            elephant.roar();
            break;
        default:
            Serial.println("NONE");
        }
    }
}
