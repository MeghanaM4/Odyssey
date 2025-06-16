#include <SPI.h>

float height = 1.85;
const int CS_PIN = 7;

void setup() {
  Serial.begin(9600);
  delay(1000);
  
  pinMode(CS_PIN, OUTPUT);
  digitalWrite(CS_PIN, HIGH); // CS idle high
  
  SPI.begin();
  SPI.beginTransaction(SPISettings(250000, MSBFIRST, SPI_MODE0)); // 250kHz
  
  Serial.println("Xiao SPI Master ready");
}

void loop() {
  String heightStr = String(height, 2);
  
  Serial.print("Sending: ");
  Serial.println(heightStr);
  
  digitalWrite(CS_PIN, LOW); // Start transmission
  delay(1); // Give slave time to detect CS
  
  // Send each character
  for (int i = 0; i < heightStr.length(); i++) {
    SPI.transfer(heightStr[i]);
    delayMicroseconds(100); // Small delay between bytes
  }
  SPI.transfer('\n'); // End marker
  
  delay(1); // Hold CS low briefly
  digitalWrite(CS_PIN, HIGH); // End transmission
  
  height += 0.01;
  delay(1000);
}