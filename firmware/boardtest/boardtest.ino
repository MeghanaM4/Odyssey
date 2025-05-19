int pin = 6;

void setup() {
  // put your setup code here, to run once:
  pinMode(pin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
    digitalWrite(pin, HIGH);
    delay(1000);
}
