#include <Wire.h>

float height = 1.85;

void setup() {
  Wire.begin();
  Wire.setClock(100000);
  Serial.begin(9600);
}

void loop() {
  String heightStr = String(height, 2);
  
  Wire.beginTransmission(4);
  delayMicroseconds(100);
  Wire.print(heightStr);
  byte error = Wire.endTransmission();

  if (error == 0) {
    Serial.print("Successfully sent: ");
    Serial.println(heightStr);
  } else {
    Serial.print("Transmission error: ");
    Serial.println(error);
  }
  
  height++;
  delay(1000);
}


