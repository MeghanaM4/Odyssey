#include <Wire.h>


void setup() {
  Wire.begin(4);

  Serial.begin(9600);

  Serial.println("started");

  Wire.setSDA(6);
  Wire.setSCL(7);
  Wire.onReceive(receiveEvent);
}

void loop() {
  delay(100);
}

void receiveEvent(int bytes) {
  String receivedData = "";
  
  while (Wire.available()) {
    char c = Wire.read();
    receivedData += c;
  }
  
  float height = receivedData.toFloat();
  Serial.print("Received height: ");
  Serial.println(height, 2);
}