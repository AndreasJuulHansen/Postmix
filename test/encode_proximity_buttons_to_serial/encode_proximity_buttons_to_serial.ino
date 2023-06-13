int proximityLastMsgTime = millis();
const int proximityPinArray[] = { 32, 33, 34 };
int proximity[3];


// Function to format a c++ array to send over Serial
void sendProximityData() {

  // Update proximity array
  for (int i = 0; i < 3; i++) {
    proximity[i] = !digitalRead(proximityPinArray[i]);
  }

  // Create JSON string
  const int arrayLength = sizeof(proximity) / sizeof(proximity[0]);
  String jsonString = "{\"proximity\": [";
  for (int i = 0; i < arrayLength; i++) {
    jsonString += String(proximity[i]);
    if (i < arrayLength - 1) {
      jsonString += ",";
    }
  }
  jsonString += "]}";
  
  // Send JSON string over serial
  Serial.println(jsonString);
}






void setup() {
  Serial.begin(115200);

  // Set relevant pins as input pullup
  for (int i = 0; i < 3; i++) {
    pinMode(proximityPinArray[i], INPUT_PULLUP);
  }
}






void loop() {

  // Send proximity values over serial
  if (millis() - proximityLastMsgTime > 1000) {
    sendProximityData();
    proximityLastMsgTime = millis();
  }
}
