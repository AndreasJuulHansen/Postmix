String input = "[[-1,-1,0],[-1,-1,0],[-1,-1,0]]";
int arr[3][3];

const int bufferSize = 64;
char buffer[bufferSize];




void updateArr(String input) {
  input.replace("[", "");
  input.replace("]", "");
  input.replace(" ", "");
  int index = 0;
  char* inputCopy = strdup(input.c_str());
  char* pch = strtok(inputCopy, ",");
  while (pch != NULL) {
    arr[index / 3][index % 3] = atoi(pch);
    pch = strtok(NULL, ",");
    index++;
  }
  free(inputCopy);
}

void updateStatus(){
	if (Serial.available() > 0) {
		int bytesRead = Serial.readBytesUntil('\n', buffer, bufferSize);
		input = String(buffer).substring(0, bytesRead);
	}
	updateArr(input);
}


//void loop() {
// 
//  if (millis() % 1000 < 5) {
//    updateStatus();
//  }
//  
//}
