// Simple CS reading test for Pico
#define CS_PIN 22

void setup() {
  Serial.begin(9600);
  delay(10000);
  
  pinMode(CS_PIN, INPUT_PULLUP);
  
  Serial.println("Pico CS Test - Reading CS pin");
  Serial.print("CS Pin: ");
  Serial.println(CS_PIN);
}

void loop() {
  static int lastState = -1;
  int currentState = digitalRead(CS_PIN);
  
  if (currentState != lastState) {
    Serial.print("CS changed to: ");
    Serial.println(currentState ? "HIGH" : "LOW");
    lastState = currentState;
  }
  
  delay(50);
}