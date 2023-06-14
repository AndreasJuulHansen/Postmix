#include <FastLED.h>

FASTLED_USING_NAMESPACE

#define DATA_PIN 25
// #define CLK_PIN   4
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS 3
CRGB leds[NUM_LEDS];

#define BRIGHTNESS 255
#define FRAMES_PER_SECOND 120

void ledSetup()
{
    // tell FastLED about the LED strip configuration
    FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    // FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

    // set master brightness control
    FastLED.setBrightness(BRIGHTNESS);
    FastLED.clear();
    Serial.begin(115200);
}


unsigned long previousMillis[NUM_LEDS] = {0};
// const long intervall = 250;      // interval at which to blink the LED (milliseconds)
int ledState[NUM_LEDS] = {LOW}; // variable to store the current state of the LED

void ledBlink(int led, CRGB color)
{
    unsigned long currentMillis = millis(); // get the current time

    if (currentMillis - previousMillis[led] >= 250)
    {                                        // check if it's time to blink the LED
        previousMillis[led] = currentMillis; // save the current time

        if (ledState[led] == LOW)
        {                         // check if the LED is off
            ledState[led] = HIGH; // turn on the LED
            leds[led] = color;
        }
        else
        {
            ledState[led] = LOW; // turn off the LED
            leds[led] = CRGB::Black;
        }
    }
}

void ledControl(int row, int col, int status)
{
    // Converts into indexes:
    int led = row * 3 + col;

    switch (status)
    {
    case -1: // Turn off
        leds[led] = CRGB::Black;
        break;

    case 0: // Green
        leds[led] = CRGB::Green;
        // Serial.print("Grøn på: ");
        // Serial.print(row);
        // Serial.println(col);
        break;

    case 1: // Orange
        leds[led] = CRGB::OrangeRed;
        // Serial.print("Orange på: ");
        // Serial.print(row);
        // Serial.println(col);
        break;

    case 2: // Blinking red
        ledBlink(led, CRGB::Red);
        // Serial.print("Rød på: ");
        // Serial.print(row);
        // Serial.println(col);
        break;

    case 3: // Blinking blue
        ledBlink(led, CRGB::Blue);
        // Serial.print("Blå på: ");
        // Serial.print(row);
        // Serial.println(col);
        break;
    }
}

// Very slim boi, but hard to understand what is going on
// void ledControl(int row, int col, int status)
// {
//     int led = row * 3 + col;
//     leds[led] = (status == 0) ? CRGB::Green : (status == 1) ? CRGB::OrangeRed : (status == 2) ? ledBlink(led, CRGB::Red, 250) : ledBlink(led, CRGB::Blue, 100);
// }

// Test function for seeing all LEDS work
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
void rainbow()
{
    // FastLED's built-in rainbow generator
    fill_rainbow(leds, NUM_LEDS, gHue, 7);
    FastLED.show();
    // insert a delay to keep the framerate modest
    FastLED.delay(1000 / FRAMES_PER_SECOND);

    // do some periodic updates
    EVERY_N_MILLISECONDS(20) { gHue++; } // slowly cycle the "base color" through the rainbow
}