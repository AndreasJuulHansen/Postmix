//int proximityLastMsgTime = millis();
const int proximityPinArray[] = {14, 27, 26};
int proximity[3];


void proximitySetup() {
    // Set relevant pins as input pullup
    for (int i = 0; i < 3; i++)
    {
        pinMode(proximityPinArray[i], INPUT_PULLUP);
    }
}

// Function to format a c++ array to send over Serial
void sendProximityData()
{

    // Update proximity array
    for (int i = 0; i < 3; i++)
    {
        proximity[i] = !digitalRead(proximityPinArray[i]);
    }

    // Create JSON string
    const int arrayLength = sizeof(proximity) / sizeof(proximity[0]);
    String jsonString = "{\"proximity\": [";
    for (int i = 0; i < arrayLength; i++)
    {
        jsonString += String(proximity[i]);
        if (i < arrayLength - 1)
        {
            jsonString += ",";
        }
    }
    jsonString += "]}";

    // Send JSON string over serial
    Serial.println(jsonString);
}





