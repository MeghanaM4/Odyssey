// Simple CS toggle test for Xiao
const int CS_PIN = 7;

void setup() {
  Serial.begin(9600);
  delay(1000);
  
  pinMode(CS_PIN, OUTPUT);
  digitalWrite(CS_PIN, HIGH);
  
  Serial.println("Xiao CS Test - Toggling CS pin every 2 seconds");
  Serial.print("CS Pin: ");
  Serial.println(CS_PIN);
}

void loop() {
  Serial.println("Setting CS LOW");
  digitalWrite(CS_PIN, LOW);
  delay(2000);
  
  Serial.println("Setting CS HIGH");  
  digitalWrite(CS_PIN, HIGH);
  delay(2000);
}