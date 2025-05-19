/*
    A
   ---
F |   | B
  | G |
   ---
E |   | C
  |   |
   ---
    D
  This example code is in the public domain.
 */
 
// Note: this organization sucks and make it better
int pinA = 6;
int pinB = 7;
int pinC  = 8;
int pinD = 9;
int pinE = 10;
int pinF = 12;
int pinG = 11;
int Dig1 = 2;
int Dig2 = 3;
int Dig3 = 4;
int Dig4 = 5;
int DP = 13;

int pins[] = {Dig1, Dig2, Dig3, Dig4, pinA, pinB, pinC, pinD, pinE, pinF, pinG, DP};
int dispPins[] = {pinA, pinB, pinC, pinD, pinE, pinF, pinG};
int digPins[] = {Dig1, Dig2, Dig3, Dig4};


void setup() {     

  for(int pin : pins) {
    pinMode(pin, OUTPUT);
  }

}

void loop() {
  for(int i = 0; i < sizeof(dispPins); i++) {
    digitalWrite(dispPins[i-1], LOW);
    delay(300);
    digitalWrite(dispPins[i], HIGH);
    delay(300);
  }

  for(int i = 0; i < sizeof(digPins); i++) {
    digitalWrite(digPins[i], HIGH);
    for(int j = 0; j < sizeof(dispPins); j++) {
      digitalWrite(dispPins[i], HIGH);
      delay(150);
    }
  }

}