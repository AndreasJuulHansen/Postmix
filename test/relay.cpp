#include <Arduino.h>
const int relayPin = 2;

void setup() {
    Serial.begin(115200);  //baudrate
    pinMode(relayPin, OUTPUT);      // initialize LED as an output
}

void loop() {
    Serial.println("Pumpe tændt");
    digitalWrite(relayPin, HIGH);   // turn relay on - Pump
    delay(10000);
    Serial.println("Pumpe slukket");
    digitalWrite(relayPin, LOW);   // turn relay on - Pump
    delay(10000);
}