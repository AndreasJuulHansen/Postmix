String input = "[[-1,-1,-1],[-1,-1,-1],[-1,-1,-1]]";
int arr[3][3];

const int bufferSize = 64;
char buffer[bufferSize];


/* // This function updates the array based on the input string.
void updateArr(String input) {
  // Remove the square brackets and spaces from the input string.
  input.replace("[", "");
  input.replace("]", "");
  input.replace(" ", "");

  // Initialize the index and create a copy of the input string.
  int index = 0;
  char* inputCopy = strdup(input.c_str());

  // Tokenize the input string using commas as delimiters.
  char* pch = strtok(inputCopy, ",");
  while (pch != NULL) {
    // Convert the token to an integer and store it in the array.
    arr[index / 3][index % 3] = atoi(pch);

    // Get the next token.
    pch = strtok(NULL, ",");

    // Increment the index.
    index++;
  }

  // Free the memory used by the copy of the input string.
  free(inputCopy);
}

// This function reads data from the serial buffer and updates the array.
void updateStatus(){
	// Check if there is data available in the serial buffer.
	if (Serial.available() > 0) {
		// Read data from the serial buffer until a newline character is encountered.
		int bytesRead = Serial.readBytesUntil('\n', buffer, bufferSize);

		// Convert the data to a string and remove any trailing whitespace.
		input = String(buffer).substring(0, bytesRead);

		// Update the array based on the input string.
		updateArr(input);
	}
} */




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