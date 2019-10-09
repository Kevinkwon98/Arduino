#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>

int a = 8;
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);

void setup(void) {
  u8g2.setDrawColor(1);
  u8g2.begin();
}

void loop(void) {
  for(a; a <= 100; a++) {
    u8g2.clearBuffer();
    u8g2.drawRFrame(12, 25, 104, 14, 4);
    u8g2.drawRBox(14, 27, a, 10, 4);
    u8g2.sendBuffer();
   }
   u8g2.clearDisplay();
   }
