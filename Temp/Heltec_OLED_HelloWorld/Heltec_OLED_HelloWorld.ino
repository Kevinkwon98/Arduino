/*
 * 보드 : NodeMCU 1.0 (ESP12)
 * 
 */

#include <Wire.h>
#include <SeeedOLED.h>

String message;

void setup()
{
  //Serial.begin(9600);  
  //while (!Serial);
  
  Wire.begin();
  SeeedOled.init();  //initialze SEEED OLED display

  SeeedOled.clearDisplay();          //clear the screen and set start position to top left corner
  SeeedOled.setNormalDisplay();      //Set display to normal mode (i.e non-inverse mode)
  SeeedOled.setPageMode();           //Set addressing mode to Page Mode
  SeeedOled.setTextXY(0,0);          //Set the cursor to Xth Page, Yth Column  
  SeeedOled.putString("Hello World!"); //Print the String
  SeeedOled.setTextXY(1,0);          //Set the cursor to Xth Page, Yth Column  
  SeeedOled.putString("Kwon Keunhyug"); //Print the String
  SeeedOled.setTextXY(2,0);          //Set the cursor to Xth Page, Yth Column  
  SeeedOled.putString("How are you?"); //Print the String
  SeeedOled.setTextXY(3,0);          //Set the cursor to Xth Page, Yth Column  
  SeeedOled.putString("I am fine and you?"); //Print the String

}

void loop()
{ /*
   if (Serial.available()>0) {
    while (Serial.available()) {
      message += (char)Serial.read();
      delayMicroseconds(1050);         // Serial 동작 H/W와 Arduino 동작속도 차이보정
    }        
      SeeedOled.setTextXY(0,0);   
      SeeedOled.putString("MSG");
  }*/
}
