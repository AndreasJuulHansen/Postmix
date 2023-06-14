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

const int flowmeterPin = 32; // Flowmeter data pin
TaskHandle_t flowmeter;

// int buttonState = 0;         // variable for reading the pushbutton status

// **********************************************
// **********************************************

void pulse() // measure the quantity of square wave
{
    waterFlow += 1.0 / 5400.0 * 1000.0; // 5880
    // Serial.print("waterFlow: ");
    // Serial.print(waterFlow);
    // Serial.println(" mL");

    if (waterFlow > 50)
    {
        Serial.println("50");
        waterFlow = 0;
    }

    
    // Serial.print("Flowmeter task - Core: ");
    // Serial.println(xPortGetCoreID());
}

void flowmeterTask(void *pvParameters)
{
    waterFlow = 0;
    // attachInterrupt(flowmeterPin, pulse, RISING); // attach interrupt to flowmeterPin
    attachInterrupt(digitalPinToInterrupt(flowmeterPin), pulse, RISING); // attach interrupt to flowmeterPin

    for (;;)
    {
        vTaskDelay(5); // delay for 1ms
        // Serial.print("Flowmeter task - Core: x");
        // Serial.println(xPortGetCoreID());
    }
}

void flowmeterSetup()
{
    // waterFlow = 0;
    // attachInterrupt(flowmeterPin, pulse, RISING);                                           // DIGITAL Pin 32: Interrupt 0
    // attachInterrupt(digitalPinToInterrupt(flowmeterPin), pulse, RISING);                                           // DIGITAL Pin 32: Interrupt 0

    xTaskCreatePinnedToCore(
        flowmeterTask,   /* Task function. */
        "flowmeterTask", /* name of task. */
        10000,           /* Stack size of task */
        NULL,            /* parameter of the task */
        1,               /* priority of the task */
        &flowmeter,      /* Task handle to keep track of created task */
        0);              /* pin task to core 1 */
}
