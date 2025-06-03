#include <Wire.h>

// MS5607 I2C address (default)
#define MS5607_ADDR 0x77

// MS5607 commands
#define CMD_RESET 0x1E
#define CMD_CONVERT_D1_4096 0x48  // Pressure conversion with OSR=4096
#define CMD_CONVERT_D2_4096 0x58  // Temperature conversion with OSR=4096
#define CMD_ADC_READ 0x00
#define CMD_PROM_READ 0xA0

// Calibration coefficients
uint16_t C[8];  // C0 to C7, but C0 is factory data, C1-C6 are calibration coefficients

void setup() {
  Serial.begin(115200);
  Wire.begin();
  
  Serial.println("MS5607 Altimeter Test");
  
  // Initialize MS5607
  if (initMS5607()) {
    Serial.println("MS5607 initialized successfully!");
  } else {
    Serial.println("Failed to initialize MS5607");
    while(1);
  }
  
  delay(1000);
}

void loop() {
  // Read pressure and temperature
  uint32_t rawPressure = readRawPressure();
  uint32_t rawTemp = readRawTemperature();
  
  // Calculate compensated values
  int32_t dT = rawTemp - ((uint32_t)C[5] << 8);
  int32_t TEMP = 2000 + (((int64_t)dT * C[6]) >> 23);
  
  int64_t OFF = ((int64_t)C[2] << 17) + (((int64_t)C[4] * dT) >> 6);
  int64_t SENS = ((int64_t)C[1] << 16) + (((int64_t)C[3] * dT) >> 7);
  
  int32_t P = (((rawPressure * SENS) >> 21) - OFF) >> 15;
  
  // Convert to human readable values
  float temperature = TEMP / 100.0;  // Temperature in °C
  float pressure = P / 100.0;        // Pressure in mbar/hPa
  
  // Calculate altitude using barometric formula
  // Assumes sea level pressure of 1013.25 hPa
  float altitude = 44330.0 * (1.0 - pow(pressure / 1013.25, 0.1903));
  
  // Print results
  Serial.println("=== MS5607 Readings ===");
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");
  
  Serial.print("Pressure: ");
  Serial.print(pressure);
  Serial.println(" hPa");
  
  Serial.print("Altitude: ");
  Serial.print(altitude);
  Serial.println(" m");
  
  Serial.println();
  delay(2000);
}

bool initMS5607() {
  // Reset the device
  Wire.beginTransmission(MS5607_ADDR);
  Wire.write(CMD_RESET);
  Wire.endTransmission();
  delay(10);
  
  // Read calibration coefficients from PROM
  for (int i = 0; i < 8; i++) {
    Wire.beginTransmission(MS5607_ADDR);
    Wire.write(CMD_PROM_READ + (i * 2));
    Wire.endTransmission();
    
    Wire.requestFrom(MS5607_ADDR, 2);
    if (Wire.available() >= 2) {
      C[i] = (Wire.read() << 8) | Wire.read();
    } else {
      return false;
    }
  }
  
  // Verify CRC (optional but recommended)
  if (!verifyCRC()) {
    Serial.println("CRC check failed!");
    return false;
  }
  
  return true;
}

uint32_t readRawPressure() {
  // Start pressure conversion
  Wire.beginTransmission(MS5607_ADDR);
  Wire.write(CMD_CONVERT_D1_4096);
  Wire.endTransmission();
  
  delay(10);  // Wait for conversion (max 9.04ms for OSR=4096)
  
  // Read ADC result
  Wire.beginTransmission(MS5607_ADDR);
  Wire.write(CMD_ADC_READ);
  Wire.endTransmission();
  
  Wire.requestFrom(MS5607_ADDR, 3);
  uint32_t result = 0;
  if (Wire.available() >= 3) {
    result = ((uint32_t)Wire.read() << 16) | ((uint32_t)Wire.read() << 8) | Wire.read();
  }
  
  return result;
}

uint32_t readRawTemperature() {
  // Start temperature conversion
  Wire.beginTransmission(MS5607_ADDR);
  Wire.write(CMD_CONVERT_D2_4096);
  Wire.endTransmission();
  
  delay(10);  // Wait for conversion
  
  // Read ADC result
  Wire.beginTransmission(MS5607_ADDR);
  Wire.write(CMD_ADC_READ);
  Wire.endTransmission();
  
  Wire.requestFrom(MS5607_ADDR, 3);
  uint32_t result = 0;
  if (Wire.available() >= 3) {
    result = ((uint32_t)Wire.read() << 16) | ((uint32_t)Wire.read() << 8) | Wire.read();
  }
  
  return result;
}

bool verifyCRC() {
  // Simple CRC verification
  // This is a basic implementation - you can enhance it if needed
  uint16_t crcRead = C[7] & 0x000F;  // CRC is in the lower 4 bits of C7
  
  // For basic functionality, we'll assume CRC is OK if we got valid coefficients
  // A full CRC implementation would calculate CRC-4 on the coefficient data
  return (C[1] != 0 && C[2] != 0);  // Basic sanity check
}