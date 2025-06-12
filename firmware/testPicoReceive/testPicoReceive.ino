#include <Wire.h>

void setup() {
  Serial.begin(9600);
  delay(3000);
  Serial.println("Testing I2C slave mode");
  
  Wire.begin(4); 
  
  Serial.println("I2C slave initialized");
}

void loop() {
  Serial.println("Slave mode running");
  delay(1000);
}