#include <LiquidCrystal.h>

LiquidCrystal lcd (12,11,2,3,4,5);

void setup() {
lcd.begin(16,2);
}

void loop () {
int light = analogRead(A0);
lcd. clear() ;
lcd. print ("Light : ");
lcd. print ( light );
delay (500);
}
