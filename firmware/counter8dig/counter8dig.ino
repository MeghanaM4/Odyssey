#include <LedControl.h>

// Pin 10 = DataIn, Pin 8 = CLK, Pin 7 = CS/LOAD
LedControl lc = LedControl(10, 8, 7, 1);  // 1 = number of MAX7219 chips

int counter = 0;

void setup() {
  lc.shutdown(0, false);       // Wake up display
  lc.setIntensity(0, 8);       // Set brightness 0–15
  lc.clearDisplay(0);          // Clear display
}

void loop() {
  displayNumber(counter);
  counter++;
  delay(1000);
}

void displayNumber(int num) {
  // Clear previous digits
  lc.clearDisplay(0);
  
  // Display from right to left
  int digit = 0;
  if (num == 0) {
    lc.setDigit(0, 0, 0, false);
  } else {
    while (num > 0 && digit < 8) {
      lc.setDigit(0, digit, num % 10, false);
      num /= 10;
      digit++;
    }
  }
}
