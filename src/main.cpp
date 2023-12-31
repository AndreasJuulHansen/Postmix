#include <Arduino.h>
#include <ArduinoJson.h>
#include "displayModule.h"
#include "flowmeterModule.h"
#include "ledModule.h"
#include "serialModule.h"
#include "proximityModule.h"

void setup()
{
    // delay(3000);
    Serial.begin(115200);
    Serial.setTimeout(100);
    // delay(3000);
    displaySetup();
    delay(2000);
    sdCardSetup();
    flowmeterSetup();
    ledSetup();
    proximitySetup();

    // pinMode(26, INPUT_PULLUP); // initialize pushbutton as an input with internal pullup resistor
    // pinMode(27, INPUT_PULLUP); // initialize pushbutton as an input with internal pullup resistor
    // pinMode(14, INPUT_PULLUP); // initialize pushbutton as an input with internal pullup resistor
}

int lastTime = millis();
// int lastStatusText;

// Default input
// type, led, percentage
// String input = "[[1, 0, 20], [5, 2, 50], [0, 3, 75]]";

// Create doc
// const size_t capacity = JSON_ARRAY_SIZE(3) + 3 * JSON_ARRAY_SIZE(3) + 60;
// DynamicJsonDocument doc(capacity);

// int buttonState1 = 0;
// int buttonState2 = 0;
// int buttonState3 = 0;

void loop()
{
    int currentTime = millis();
    // Serial.print("Normal task - Core: ");
    // Serial.println(xPortGetCoreID());

    // Check for serial msg
    // if (Serial.available())
    // {
    //     // Serial.println("Received input.");
    //     input = Serial.readString();
    //     // Serial.println(input);
    // }

    // // Update doc
    // deserializeJson(doc, input);

    // Create arr instead of doc (might be unnecessary)
    // int arr[3][3];
    // for (int i = 0; i < 3; i++)
    // {
    //     for (int j = 0; j < 3; j++)
    //     {
    //         arr[i][j] = doc[i][j];
    //     }
    // }

    ////////////////////////////////////////////////

    if (currentTime - lastTime > 1000)
    {
        lastTime = millis();
        // Serial.println("Input:");
        // Serial.println(input);

        // Send proximity data over Serial
        sendProximityData();

        // Check serial and update Arr
        updateStatus();

        // Update
        for (int i = 0; i < 2; i++)
        {
            loadType(0, i, arr[i][0]);
            drawLiquid(0, i, arr[i][2]);
        }
    }

    for (int i = 0; i < 3; i++)
    {
        ledControl(0, i, arr[2 - i][1]);
    }
    FastLED.show();
}
