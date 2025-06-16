#include <Wire.h>

float height = 1.85;

void setup() {
  Wire1.begin();
  Wire1.setClock(50000);
  Serial.begin(9600);
}

void loop() {
  String heightStr = String(height, 2);
  
  Wire1.beginTransmission(8);
  Wire1.write(heightStr.c_str(), heightStr.length());
  byte error = Wire1.endTransmission();

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


