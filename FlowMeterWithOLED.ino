/*
  Sensirion SFM3300 Flow Sensor interfacing
  Syed Razwanul Haque(Nabil), https://github.com/Nabilphysics/SensirionSFM3000Arduino/
  https://www.nabilbd.com
  https://www.cruxbd.com
  Sensor Datasheet: https://www.sensirion.com/fileadmin/user_upload/customers/sensirion/Dokumente/5_Mass_Flow_Meters/Datasheets/Sensirion_Mass_Flow_Meters_SFM3300_Datasheet.pdf
  Credit:
  SFM3300 Modified From: https://www.14core.com/wiring-sfm3000-air-gas-flow-meter/
  
  OLED DISPLAY: SSD1306 128*64 from Ardafruit
  
  This is an example for our Monochrome OLEDs based on SSD1306 drivers. Pick one up today in the adafruit shop! ------> http://www.adafruit.com/category/63_98
  This example is for a 128x32 pixel display using I2C to communicate 3 pins are required to interface (two I2C and one reset).
  Adafruit invests time and resources providing this open source code, please support Adafruit and open-source hardware by purchasing products from Adafruit!
  Written by Limor Fried/Ladyada for Adafruit Industries, with contributions from the open source community. BSD license, check license.txt for more information All text above, and the splash screen below must be included in any redistribution.

  Flow Meter using Sensirion SFM3300 and OLED Written by Syed Razwanul Haque(Nabil) for www.cruxbd.com, with contributions from the open source community. BSD license, check license.txt for more information All text above, 
  and the cruxDisplay() screen below must be included in any redistribution.

***** HardWare Connection ***** 
 Arduino, SFM3300 Flow Sensor and SSD1306 OLED Display
 
 Sensor Power > Arduino sensorPowerPin or Use This pin for transistor switching(Trigger), 
 Sensor Data > Arduino SDA 
 Sensor Clock > Arduino SCK
 GND > GND
 OLED SDA > Arduino SDA
 OLED SCK > Arduino SCK
 OLED VDD > +5V
 OLED GND > GND
*/
#include <Wire.h>
#include "SFM3300.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

SFM3300 sensirionFlow(64);  

long int sensirion3300Offset = 32768; // Offset for Sensirion 3300 Flow Sensor
float scaleFactor = 120.0; //For Air
const float flowErrorSLM = 0.3;
uint8_t sensorPowerPin = 5; // Since Sensor only consume 5mw power we could power it from Microcontroller Pin, You may use Transistor Switching
int ret;

//#define WINDOW_SIZE 5
//
//int INDEX = 0;
//float VALUE = 0;
//float SUM = 0;
//float READINGS[WINDOW_SIZE];
//float AVERAGED = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  pinMode(sensorPowerPin, OUTPUT);
  sensirionFlow.hardReset(sensorPowerPin);
  delay(1000);
  sensirionFlow.init();
  delay(1000);
  Serial.println("Sensor initialized!");
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }


  display.display();
  delay(1000);
  display.clearDisplay();
  cruxDisplay();
  display.display();
  delay(3000);
  // Clear the buffer
  display.clearDisplay();
  display.display();
  delay(500);

}

void loop() {
/////////////// Flow Measure ///////////////

  unsigned int result = sensirionFlow.getvalue();
  float flow = ((float(result) - sensirion3300Offset) / scaleFactor)- flowErrorSLM;
  Serial.println(flow);
  
////////////////// Temperature Measure /////////////
//  unsigned int temp = sensirionFlow.tempRead();
//  float actualTemp = (float(temp) - 20000) / 100;
//  Serial.print(" , ");
//  Serial.println(actualTemp);


//////// Error Handling ////////////////////////////
  if (flow < -270.00) {
    sensirionFlow.hardReset(sensorPowerPin); //Sensor Power ON/OFF
    sensirionFlow.init();
    delay(1000);
  }
//  SUM = SUM - READINGS[INDEX];       // Remove the oldest entry from the sum
//  VALUE = flow;        // Read the next sensor value
//  READINGS[INDEX] = VALUE;           // Add the newest reading to the window
//  SUM = SUM + VALUE;                 // Add the newest reading to the sum
//  INDEX = (INDEX+1) % WINDOW_SIZE;   // Increment the index, and wrap to 0 if it exceeds the window size
// 
//  AVERAGED = SUM / WINDOW_SIZE;      // Divide the sum of the window by the window size for the result
 

  
  display.clearDisplay();
////1st Line
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(5, 0); //x,y
  display.print(flow);
  display.setTextSize(1);
  display.setCursor(92, 5);
  display.println(F(" SLM"));

////2nd Line
  display.setTextSize(2);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 20);
  display.print(flow * 16.66); //1000/60  flow
 
  display.setTextSize(1);
  display.setCursor(93, 25);
  display.println(F(" ml/S"));

///// 3rd Line
//  display.setTextSize(2);      // Normal 1:1 pixel scale
//  display.setTextColor(SSD1306_WHITE); // Draw white text
//  display.setCursor(20, 50);
//  display.print(actualTemp); //1000/60
//  display.setTextSize(1);
//  display.setCursor(93, 55);
//  display.println(F("deg C"));
//  

  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(15, 55);
  display.print(F("www.cruxbd.com")); //1000/60  flow
  
  display.display();
  delay(100);
}

void cruxDisplay(){
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);
  display.println(F("CRUXBD.COM"));
  display.setTextSize(2);
  display.setCursor(0, 30);
  display.println(F("Flow Meter"));
}
