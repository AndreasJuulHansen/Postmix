int proximityLastMsgTime = millis();
const int proximityPinArray[] = {14, 27, 26};
int proximity[3];
int arr[3][3];


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

// Function to update the array containing the status for each pos on the rack
void updateArray()
{
    // Create doc
    const size_t capacity = JSON_ARRAY_SIZE(3) + 3 * JSON_ARRAY_SIZE(3) + 60;
    DynamicJsonDocument doc(capacity);
    // Update doc
    deserializeJson(doc, input);

    // Create arr instead of doc (might be unnecessary)
    int arr[3][3];
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            arr[i][j] = doc[i][j];
        }
    }
}



/* // Send proximity values over serial
    if (millis() - proximityLastMsgTime > 1000)
    {
        proximityLastMsgTime = millis();
        sendProximityData();
        
        for (int x = 0; x <= 2; x++) {
            for (int y = 0; y <= 2; y++) {
                Serial.print(arr[x][y]);
            }
        }
    } */