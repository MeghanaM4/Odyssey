// Software SPI implementation for modified Pico
#define MISO_PIN 25  // Your modified board's MISO
#define MOSI_PIN 21  // Your modified board's MOSI  
#define SCK_PIN 24   // Your modified board's SCK
#define CS_PIN 22    // Your modified board's CS

volatile bool dataAvailable = false;
String receivedData = "";

void csInterrupt() {
  dataAvailable = true;
}

void setup() {
  Serial.begin(9600);
  delay(10000); //waiting for serial
  
  Serial.println("=== Modified Pico - Software SPI Slave ===");
  Serial.print("MISO: "); Serial.print(MISO_PIN);
  Serial.print(", MOSI: "); Serial.print(MOSI_PIN);
  Serial.print(", SCK: "); Serial.print(SCK_PIN);
  Serial.print(", CS: "); Serial.println(CS_PIN);
  
  // Configure pins manually
  pinMode(MISO_PIN, OUTPUT);      // We send data on MISO
  pinMode(MOSI_PIN, INPUT);       // We receive data on MOSI
  pinMode(SCK_PIN, INPUT);        // Clock from master
  pinMode(CS_PIN, INPUT_PULLUP);  // Chip select
  
  // Set initial states
  digitalWrite(MISO_PIN, LOW);
  
  // Attach interrupt to CS
  attachInterrupt(digitalPinToInterrupt(CS_PIN), csInterrupt, FALLING);
  
  Serial.println("Software SPI slave ready!");
  Serial.println("Waiting for data...");
}

uint8_t softwareSPI_transfer() {
  uint8_t receivedByte = 0;
  
  // Wait for 8 clock cycles
  for (int bit = 7; bit >= 0; bit--) {
    // Wait for clock to go high
    while (digitalRead(SCK_PIN) == LOW && digitalRead(CS_PIN) == LOW) {
      delayMicroseconds(1);
    }
    
    if (digitalRead(CS_PIN) == HIGH) break; // CS went high, abort
    
    // Read bit on MOSI
    if (digitalRead(MOSI_PIN) == HIGH) {
      receivedByte |= (1 << bit);
    }
    
    // Wait for clock to go low
    while (digitalRead(SCK_PIN) == HIGH && digitalRead(CS_PIN) == LOW) {
      delayMicroseconds(1);
    }
    
    if (digitalRead(CS_PIN) == HIGH) break; // CS went high, abort
  }
  
  return receivedByte;
}

void loop() {
  static unsigned long lastHeartbeat = 0;
  
  // Heartbeat
  if (millis() - lastHeartbeat > 3000) {
    Serial.print("Heartbeat - CS: ");
    Serial.println(digitalRead(CS_PIN) ? "HIGH" : "LOW");
    lastHeartbeat = millis();
  }
  
  if (dataAvailable) {
    Serial.println("*** CS INTERRUPT - Starting Reception ***");
    
    // Small delay to let master get ready
    delayMicroseconds(100);
    
    if (digitalRead(CS_PIN) == LOW) {
      receivedData = "";
      int byteCount = 0;
      
      Serial.println("CS is LOW - receiving data...");
      
      // Receive data while CS is low
      while (digitalRead(CS_PIN) == LOW && byteCount < 20) {
        uint8_t receivedByte = softwareSPI_transfer();
        
        Serial.print("Byte ");
        Serial.print(byteCount);
        Serial.print(": 0x");
        Serial.print(receivedByte, HEX);
        Serial.print(" ('");
        Serial.print((char)receivedByte);
        Serial.println("')");
        
        if (receivedByte == '\n') {
          Serial.println("End marker found!");
          break;
        } else if (receivedByte != 0) {
          receivedData += (char)receivedByte;
        }
        
        byteCount++;
      }
      
      if (receivedData.length() > 0) {
        Serial.print("Complete message: '");
        Serial.print(receivedData);
        Serial.println("'");
        
        float height = receivedData.toFloat();
        Serial.print("Parsed height: ");
        Serial.println(height, 2);
      } else {
        Serial.println("No valid data received");
      }
    }
    
    Serial.println("*** Reception complete ***");
    dataAvailable = false;
  }
  
  delay(10);
}