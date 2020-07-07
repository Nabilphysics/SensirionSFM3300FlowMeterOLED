/*
  Sensirion SFM3300 Flow Sensor interfacing
  https://www.nabilbd.com
  https://www.cruxbd.com
  Credit:
  Modified From: https://www.14core.com/wiring-sfm3000-air-gas-flow-meter/
*/

#include "SFM3300.h"

SFM3300::SFM3300(int i2cAddress)
{
  mI2cAddress = i2cAddress;
}

void SFM3300::init()
{
  int a = 0;
  int b = 0;
  int c = 0; 
  Wire.beginTransmission(byte(mI2cAddress)); // transmit to device with I2C mI2cAddress
  Wire.beginTransmission(byte(mI2cAddress)); 
  Wire.write(byte(0x10));      
  Wire.write(byte(0x00));     
  Wire.endTransmission();
  delay(5);
  
//    Wire.beginTransmission(byte(64));
//    Wire.write(0x10);
//    Wire.write(0x00);
//    Wire.endTransmission();
//    delay(5);
    
  Wire.requestFrom(mI2cAddress, 3); //
  a = Wire.read(); // received first byte stored here
  b = Wire.read(); // received second byte stored here
  c = Wire.read(); // received third byte stored here

  Wire.endTransmission();
  delay(5);

}
 
float SFM3300::getvalue()
{
//  Wire.beginTransmission(byte(64));
//  Wire.write(byte(0x10));      
//  Wire.write(byte(0x00)); 
//  Wire.endTransmission();
  Wire.requestFrom(mI2cAddress, 3); // set read 3 bytes from device with address 0x40
  uint16_t a = Wire.read(); // received first byte stored here. The variable "uint16_t" can hold 2 bytes, this will be relevant later
  uint8_t b = Wire.read(); // second received byte stored here
  a = (a << 8) | b; // combine the two received bytes to a 16bit integer value
  int Flow=a;
  return Flow;
}

void SFM3300::softReset()
{   
    int ret;
    do {
    // Soft reset the sensor
    Wire.beginTransmission(byte(64));
    Wire.write(0x20);
    Wire.write(0x00);
    ret = Wire.endTransmission();
    if (ret != 0) {
      Serial.println("Error while sending soft reset command, retrying...");
      delay(500); // wait long enough for chip reset to complete
    }
  } while (ret != 0);
  if(ret == 0){
    Serial.println("Soft Reset Done");
  }
}
void SFM3300::hardReset(uint8_t sensorPowerPin)
{   digitalWrite(sensorPowerPin, LOW);
    delay(500);
    digitalWrite(sensorPowerPin, HIGH);
    delay(500);
    int ret;
    do {
    // Soft reset the sensor
    Wire.beginTransmission(byte(64));
    Wire.write(0x20);
    Wire.write(0x00);
    ret = Wire.endTransmission();
    if (ret != 0) {
      Serial.println("Error while sending soft reset command, retrying...");
      delay(500); // wait long enough for chip reset to complete
    }
  } while (ret != 0);
  if(ret == 0){
    Serial.println("Hard Reset Done");
  }
}
float SFM3300::tempRead(){
    Wire.beginTransmission(byte(64));
    Wire.write(0x10);
    Wire.write(0x01);
    Wire.endTransmission();
    delay(5);
    Wire.requestFrom(mI2cAddress, 3); // set read 3 bytes from device with address 0x40
    uint16_t x = Wire.read(); // received first byte stored here. The variable "uint16_t" can hold 2 bytes, this will be relevant later
    uint16_t y = Wire.read();
    x = (x << 8) | y;
    int temp = x;
    return temp;
}
