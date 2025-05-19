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
int segPins[] = {pinA, pinB, pinC, pinD, pinE, pinF, pinG};
int digPins[] = {Dig1, Dig2, Dig3, Dig4};

const uint8_t numPatterns[10] = {
  0x3F, // 0 => segments a,b,c,d,e,f
  0x06, // 1 => segments b,c
  0x5B, // 2 => a,b,d,e,g
  0x4F, // 3 => a,b,c,d,g
  0x66, // 4 => b,c,f,g
  0x6D, // 5 => a,c,d,f,g
  0x7D, // 6 => a,c,d,e,f,g
  0x07, // 7 => a,b,c
  0x7F, // 8 => a,b,c,d,e,f,g
  0x6F  // 9 => a,b,c,d,f,g
};

int pow10[] = {1, 10, 100, 1000};

void setup() {     

  for(int pin : pins) {
    pinMode(pin, OUTPUT);
  }

  turnOffSegs();
  turnOffDigs();
}

void loop() {
  for (int i = 0; i < 2048; i++) {
    unsigned long tStart = millis();
    while (millis() - tStart < 200) {
      refreshDisplay(i); // keep showing the number for 200ms
    }
  }
}


void turnOffDigs() {
  for(int pin : digPins) {
    digitalWrite(pin, HIGH);
  }
}

void turnOffSegs() {
  for(int pin : segPins) {
    digitalWrite(pin, LOW);
  }
}

void setSegs(uint8_t pattern) {
  for (int i = 0; i < 7; i++) {
    bool segmentOn = pattern & (1 << i);
    digitalWrite(segPins[i], segmentOn ? HIGH : LOW);
  }
}

void displayDigit(int digit, int position) {
  if (digit < 0 || digit > 9 || position < 0 || position > 3) return;
  turnOffDigs();
  setSegs(numPatterns[digit]);
  digitalWrite(digPins[position], LOW); // Enable digit (common cathode)
  delay(5);
  digitalWrite(digPins[position], HIGH); // Disable digit after delay
}

void displayNum(int number) {
  if (number < 0) number = -number;
  if (number > 9999) number = 9999;

  for (int i = 3; i >= 0; i--) {
    int digit = number % 10;   
    displayDigit(digit, i);        
    number /= 10;
  }
}

void refreshDisplay(int number) {
  for (int pos = 0; pos < 4; pos++) {
    int digit = (number / pow10[3 - pos]) % 10;

    turnOffDigs();
    setSegs(numPatterns[digit]);
    digitalWrite(digPins[pos], LOW);

    delayMicroseconds(1000);
  }
}

