#include <Arduino.h>

// void setup() {
//   // put your setup code here, to run once:
// }

// void loop() {
//   // put your main code here, to run repeatedly:
// }

// int flowPin = 32; // Pin connected to flowmeter
// int flowRate; // Flow rate
// unsigned int flowMilliLitres;
// unsigned long totalMilliLitres;

// void setup() {
//   pinMode(flowPin, INPUT);
//   Serial.begin(115200);
// }

// void loop() {
//   flowRate = pulseIn(flowPin, HIGH);
//   flowMilliLitres = (flowRate * 60 / 5880); // Convert frequency to flow rate
//   totalMilliLitres += flowMilliLitres;
//   Serial.print("Flow rate: ");
//   Serial.print(flowMilliLitres);
//   Serial.print("mL/minute");
//   Serial.print("  Total flow: ");
//   Serial.print(totalMilliLitres);
//   Serial.println("mL");
// }

// /*
//  * Water flow meter with ESP32
//  * Code adapted from https://github.com/ControlEverythingCommunity/YF-S401-Flow-Sensor/blob/master/Arduino/YF-S401.ino
//  */

// volatile int pulseCount; // Number of pulses
// float flowRate; // Flow rate in mL/min
// float flowMilliLitres; // Total volume in mL
// unsigned long totalMilliLitres; // Total volume in mL
// unsigned long oldTime;

// int flowPin = 32; // Pin connected to flowmeter

// void pulseCounter() {
//   pulseCount++;
// }

// void setup() {
//   pinMode(flowPin, INPUT_PULLUP);
//   attachInterrupt(digitalPinToInterrupt(flowPin), pulseCounter, FALLING);
//   Serial.begin(115200);
//   oldTime = millis();
// }

// void loop() {
//   if ((millis() - oldTime) > 1000) { // Only process counters once per second
//     detachInterrupt(digitalPinToInterrupt(flowPin));
//     flowRate = (pulseCount / 5880.0) * 1000 * 60 / 1000;
//     pulseCount = 0;
//     oldTime = millis();
//     flowMilliLitres = (flowRate / 60) * 1000;
//     totalMilliLitres += flowMilliLitres;
//     Serial.print("Flow rate: ");
//     Serial.print(flowRate);
//     Serial.print(" mL/min");
//     Serial.print("\t");
//     Serial.print("Total volume: ");
//     Serial.print(totalMilliLitres);
//     Serial.println(" mL");
//     attachInterrupt(digitalPinToInterrupt(flowPin), pulseCounter, FALLING);
//   }
// }


// /*
//  * Water flow meter with ESP32
//  * Code adapted from https://github.com/ControlEverythingCommunity/YF-S401-Flow-Sensor/blob/master/Arduino/YF-S401.ino
//  */

// volatile int pulseCount; // Number of pulses
// float flowRate; // Flow rate in mL/min
// float flowMilliLitres; // Total volume in mL
// unsigned long totalMilliLitres; // Total volume in mL
// unsigned long oldTime;

// int flowPin = 32; // Pin connected to flowmeter

// void pulseCounter() {
//   pulseCount++;
// }

// void setup() {
//   pinMode(flowPin, INPUT_PULLUP);
//   attachInterrupt(digitalPinToInterrupt(flowPin), pulseCounter, FALLING);
//   Serial.begin(115200);
//   oldTime = millis();
// }

// void loop() {
//   if ((millis() - oldTime) > 1000) { // Only process counters once per second
//     detachInterrupt(digitalPinToInterrupt(flowPin));
//     flowRate = (pulseCount / 5880.0) * 1000 * 60;
//     pulseCount = 0;
//     oldTime = millis();
//     flowMilliLitres = (flowRate / 60) * 1000;
//     totalMilliLitres += flowMilliLitres;
//     Serial.print("Flow rate: ");
//     Serial.print(flowRate);
//     Serial.print(" mL/min");
//     Serial.print("\t");
//     Serial.print("Total volume: ");
//     Serial.print(totalMilliLitres);
//     Serial.println(" mL");
//     attachInterrupt(digitalPinToInterrupt(flowPin), pulseCounter, FALLING);
//   }
// }


/***************************************************
 This example reads Water flow sensor Sensor.

 Created 2016-3-13
 By berinie Chen <bernie.chen@dfrobot.com>

 GNU Lesser General Public License.
 See <http://www.gnu.org/licenses/> for details.
 All above must be included in any redistribution
 ****************************************************/

/***********Notice and Trouble shooting***************
 1.Connection and Diagram can be found here  https://www.dfrobot.com/wiki/index.php?title=Water_Flow_Sensor_-_1/8%22_SKU:_SEN0216
 2.This code is tested on Arduino Uno.
 ****************************************************/
volatile double waterFlow;

void pulse()   //measure the quantity of square wave
{
  waterFlow += 1.0 / 5400.0 * 1000.0; // 5880
}

void setup() {
  Serial.begin(115200);  //baudrate
  waterFlow = 0;
  attachInterrupt(32, pulse, RISING);  //DIGITAL Pin 2: Interrupt 0
}
void loop() {
  Serial.print("waterFlow:");
  Serial.print(waterFlow);
  Serial.println(" mL");
  delay(500);
}
