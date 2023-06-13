void setup() {
  Serial.begin(115200);
}

void loop() {
  String input = "[[0, 2, 0], [3, 0, 25], [0, -1, 0]]";
  int arr[3][3];

  if (Serial.available()) {
    input = Serial.readStringUntil(']'); // Read until the end of the array
  }

  int i = 0;
  while (input.indexOf('[') >= 0) {
      int start = input.indexOf('[');
      int end = input.indexOf(']');
      String row = input.substring(start + 1, end);
      int j = 0;
      while (row.indexOf(',') >= 0) {
          int comma = row.indexOf(',');
          arr[i][j] = row.substring(0, comma).toInt();
          row = row.substring(comma + 1);
          j++;
      }
      arr[i][j] = row.toInt();
      i++;
      input = input.substring(end + 1);
  }


  ////////////////////////////////////////////////

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
        Serial.print(arr[i][j]);
        Serial.print(" ");
    }
    Serial.println();
  }
  Serial.println();
  delay(2000);


}
