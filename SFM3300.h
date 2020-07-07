/*

  Note:
  int offset = 32768; // Offset for the sensor
  float scale = 120.0; // Scale factor for Air 
  Flow = (readvalue - offset) / scale

*/

#ifndef SFM3300_h
#define SFM3300_h


class SFM3300 {
  public:
   
    SFM3300(int i2cAddress);
    void init();
    float getvalue();
    void softReset();
    void hardReset(uint8_t sensorPowerPin);
    float tempRead();


  private:

    int mI2cAddress;
    
};


#endif
