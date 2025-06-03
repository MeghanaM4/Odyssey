//open Serial Plotter and plot

#include <Wire.h>

// MS5607 Pressure/Temperature sensor
#define MS5607_ADDR 0x77
#define CMD_RESET 0x1E
#define CMD_CONVERT_D1_4096 0x48
#define CMD_CONVERT_D2_4096 0x58
#define CMD_ADC_READ 0x00
#define CMD_PROM_READ 0xA0
uint16_t C[8];

// MPU6050 Gyroscope/Accelerometer
#define MPU6050_ADDR 0x68
#define MPU6050_PWR_MGMT_1 0x6B
#define MPU6050_ACCEL_XOUT_H 0x3B
#define MPU6050_GYRO_XOUT_H 0x43
#define MPU6050_TEMP_OUT_H 0x41

// ALS31313 3D Hall Effect sensor
#define ALS31313_ADDR 0x60
#define ALS31313_X_LSB 0x28
#define ALS31313_X_MSB 0x29
#define ALS31313_Y_LSB 0x2A
#define ALS31313_Y_MSB 0x2B
#define ALS31313_Z_LSB 0x2C
#define ALS31313_Z_MSB 0x2D

// Global variables to store sensor data
float temperature = 0, pressure = 0, altitude = 0;
float accelX = 0, accelY = 0, accelZ = 0;
float gyroX = 0, gyroY = 0, gyroZ = 0;
float magX = 0, magY = 0, magZ = 0, magMagnitude = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  
  // Initialize sensors (simplified for plotter)
  initMS5607();
  initMPU6050();
  initALS31313();
  
  // Print column headers for Serial Plotter
  Serial.println("Temp_C,Pressure_hPa,Altitude_m,AccelX_g,AccelY_g,AccelZ_g,GyroX_dps,GyroY_dps,GyroZ_dps,MagX_G,MagY_G,MagZ_G,MagMag_G");
  
  delay(2000);
}

void loop() {
  // Read all sensors
  readMS5607();
  readMPU6050();
  readALS31313();
  
  // Output comma-separated values for Serial Plotter
  Serial.print("Temperature:"); Serial.print(temperature); Serial.print(",");
  Serial.print("Pressure:"); Serial.print(pressure); Serial.print(",");
  Serial.print("Altitude:"); Serial.print(altitude); Serial.print(",");
  Serial.print("AccelX:"); Serial.print(accelX); Serial.print(",");
  Serial.print("AccelY:"); Serial.print(accelY); Serial.print(",");
  Serial.print("AccelZ:"); Serial.print(accelZ); Serial.print(",");
  Serial.print("Rotation X:"); Serial.print(gyroX); Serial.print(",");
  Serial.print("Rotation Y:"); Serial.print(gyroY); Serial.print(",");
  Serial.print("Rotation Z:"); Serial.print(gyroZ); Serial.print(",");
  Serial.print("MagX:"); Serial.print(magX); Serial.print(",");
  Serial.print("MayY:"); Serial.print(magY); Serial.print(",");
  Serial.print("MagZ:"); Serial.print(magZ); Serial.print(",");
  Serial.print("Magnetic Magnitude:"); Serial.println(magMagnitude);
  
  delay(100); // Faster update rate for plotter
}

// ===== MS5607 Functions =====
bool initMS5607() {
  Wire.beginTransmission(MS5607_ADDR);
  Wire.write(CMD_RESET);
  if (Wire.endTransmission() != 0) return false;
  delay(10);
  
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
  return true;
}

void readMS5607() {
  uint32_t rawPressure = readRawPressure();
  uint32_t rawTemp = readRawTemperature();
  
  int32_t dT = rawTemp - ((uint32_t)C[5] << 8);
  int32_t TEMP = 2000 + (((int64_t)dT * C[6]) >> 23);
  
  int64_t OFF = ((int64_t)C[2] << 17) + (((int64_t)C[4] * dT) >> 6);
  int64_t SENS = ((int64_t)C[1] << 16) + (((int64_t)C[3] * dT) >> 7);
  
  int32_t P = (((rawPressure * SENS) >> 21) - OFF) >> 15;
  
  temperature = TEMP / 100.0;
  pressure = P / 100.0;
  altitude = 44330.0 * (1.0 - pow(pressure / 1013.25, 0.1903));
}

uint32_t readRawPressure() {
  Wire.beginTransmission(MS5607_ADDR);
  Wire.write(CMD_CONVERT_D1_4096);
  Wire.endTransmission();
  delay(10);
  
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
  Wire.beginTransmission(MS5607_ADDR);
  Wire.write(CMD_CONVERT_D2_4096);
  Wire.endTransmission();
  delay(10);
  
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

// ===== MPU6050 Functions =====
bool initMPU6050() {
  Wire.beginTransmission(MPU6050_ADDR);
  if (Wire.endTransmission() != 0) return false;
  
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(MPU6050_PWR_MGMT_1);
  Wire.write(0);
  return (Wire.endTransmission() == 0);
}

void readMPU6050() {
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(MPU6050_ACCEL_XOUT_H);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6050_ADDR, 14, true);
  
  if (Wire.available() >= 14) {
    int16_t axRaw = Wire.read() << 8 | Wire.read();
    int16_t ayRaw = Wire.read() << 8 | Wire.read();
    int16_t azRaw = Wire.read() << 8 | Wire.read();
    
    Wire.read(); Wire.read(); // Skip temperature readings
    
    int16_t gxRaw = Wire.read() << 8 | Wire.read();
    int16_t gyRaw = Wire.read() << 8 | Wire.read();
    int16_t gzRaw = Wire.read() << 8 | Wire.read();
    
    accelX = axRaw / 16384.0;
    accelY = ayRaw / 16384.0;
    accelZ = azRaw / 16384.0;
    
    gyroX = gxRaw / 131.0;
    gyroY = gyRaw / 131.0;
    gyroZ = gzRaw / 131.0;
  }
}

// ===== ALS31313 Functions =====
bool initALS31313() {
  Wire.beginTransmission(ALS31313_ADDR);
  return (Wire.endTransmission() == 0);
}

void readALS31313() {
  int16_t magXRaw = readALS31313Register(ALS31313_X_MSB, ALS31313_X_LSB);
  int16_t magYRaw = readALS31313Register(ALS31313_Y_MSB, ALS31313_Y_LSB);
  int16_t magZRaw = readALS31313Register(ALS31313_Z_MSB, ALS31313_Z_LSB);
  
  magX = magXRaw * 0.0001;
  magY = magYRaw * 0.0001;
  magZ = magZRaw * 0.0001;
  
  magMagnitude = sqrt(magX*magX + magY*magY + magZ*magZ);
}

int16_t readALS31313Register(uint8_t regMSB, uint8_t regLSB) {
  uint8_t msb = 0, lsb = 0;
  
  Wire.beginTransmission(ALS31313_ADDR);
  Wire.write(regMSB);
  Wire.endTransmission();
  Wire.requestFrom(ALS31313_ADDR, 1);
  if (Wire.available()) msb = Wire.read();
  
  Wire.beginTransmission(ALS31313_ADDR);
  Wire.write(regLSB);
  Wire.endTransmission();
  Wire.requestFrom(ALS31313_ADDR, 1);
  if (Wire.available()) lsb = Wire.read();
  
  return (int16_t)((msb << 8) | lsb);
}