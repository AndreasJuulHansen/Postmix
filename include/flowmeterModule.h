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

const int flowmeterPin = 32;  // Flowmeter data pin
const int buttonPin = 17;     // the number of the pushbutton pin
const int ledPin = 13;      // the number of the LED pin
const int relayPin = 2; // relay / Pump

// int buttonState = 0;         // variable for reading the pushbutton status

// **********************************************
// **********************************************

void pulse()   //measure the quantity of square wave
{
  waterFlow += 1.0 / 5400.0 * 1000.0; // 5880
}

void flowmeterSetup() {
    waterFlow = 0;
    attachInterrupt(flowmeterPin, pulse, RISING);  //DIGITAL Pin 32: Interrupt 0
    pinMode(ledPin, OUTPUT);      // initialize LED as an output
    // pinMode(buttonPin, INPUT_PULLUP); // initialize pushbutton as an input with internal pullup resistor
    pinMode(relayPin, OUTPUT);      // initialize LED as an output
}


void flowmeterCheck() {
    Serial.print("waterFlow: ");
    Serial.print(waterFlow);
    Serial.println(" mL");
}

/* void loop() {
    Serial.print("waterFlow: ");
    Serial.print(waterFlow);
    Serial.println(" mL");
    delay(500);

    buttonState = digitalRead(buttonPin); // read the state of the pushbutton value

    // if (buttonState == LOW) {      // check if pushbutton is pressed
    //     // digitalWrite(ledPin, HIGH);   // turn LED on
    //     // waterFlow = 0;
    // } else {
    //     // digitalWrite(ledPin, LOW);    // turn LED off

    if (waterFlow >= 1000.0){
        digitalWrite(ledPin, HIGH);   // turn LED on
        digitalWrite(relayPin, LOW);   // turn relay on - Pump
    } else {
        digitalWrite(relayPin, HIGH);   // turn relay on - Pump
    }

    // int val = analogRead(2);
} */