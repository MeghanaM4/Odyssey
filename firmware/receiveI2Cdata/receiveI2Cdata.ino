#include <Wire.h>

const int LED = 23;

void setup() {
  pinMode(LED, OUTPUT);

  digitalWrite(LED, HIGH);
  delay(1000);
  digitalWrite(LED, LOW);
  delay(1000);
  digitalWrite(LED, HIGH);
  delay(1000);
  digitalWrite(LED, LOW);
  delay(1000);

  Wire1.setSDA(6);
  Wire1.setSCL(7);
  Wire1.begin(8);

  digitalWrite(LED, HIGH);
  delay(100);
  digitalWrite(LED, LOW);
  delay(100);
  digitalWrite(LED, HIGH);
  delay(100);
  digitalWrite(LED, LOW);
  delay(1000);

  Wire1.onReceive(receiveEvent);
}

void loop() {
  delay(100);
}

void receiveEvent(int bytes) {
  digitalWrite(LED, HIGH);
  delay(100);
  digitalWrite(LED, LOW);
  delay(100);
  digitalWrite(LED, HIGH);
  delay(100);
  digitalWrite(LED, LOW);

  String receivedData = "";

  while (Wire1.available()) {
    char c = Wire1.read();
    receivedData += c;
  }

  if (receivedData.length() > 0) {
    float height = receivedData.toFloat();
    digitalWrite(LED, HIGH);
    delay(50);
    digitalWrite(LED, LOW);
    delay(50);
  } else {
    digitalWrite(LED, HIGH);
    delay(1000);
    digitalWrite(LED, LOW);
    delay(1000);
  }
}