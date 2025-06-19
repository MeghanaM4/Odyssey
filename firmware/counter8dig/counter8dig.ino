#include <LedControl.h>

//this crap is diabolical I cannot believe pin assignments are like this
#define DATA_IN 26 //DIN - D0
#define CLK     0   //SCK - D6
#define CS      1   //CS - D7

LedControl lc = LedControl(DATA_IN, CLK, CS, 1);

void setup() {
  lc.shutdown(0, false);
  lc.setIntensity(0, 8);
  lc.clearDisplay(0);

}

void loop() {
  static unsigned long counter = 0;
  displayNumber(counter);
  counter++;
  delay(250);
}

void displayNumber(unsigned long num) {
  for (int i = 0; i < 8; i++) {
    int digit = num % 10;
    lc.setDigit(0, i, digit, false);
    num /= 10;
  }
}
