/*
 * IRremote: IRrecvDemo - demonstrates receiving IR codes with IRrecv
 * An IR detector/demodulator must be connected to the input RECV_PIN.
 * Version 0.1 July, 2009
 * Copyright 2009 Ken Shirriff
 * http://arcfn.com
 */

#include <IRremote.h>

int RECV_PIN = 11;

IRrecv irrecv(RECV_PIN);

decode_results results;

void setup()
{
  Serial.begin(9600);
  // In case the interrupt driver crashes on setup, give a clue
  // to the user what's going on.
  Serial.println("Enabling IRin");
  irrecv.enableIRIn(); // Start the receiver
  Serial.println("Enabled IRin");
  pinMode( 8, OUTPUT);
  pinMode( 9, OUTPUT);
  pinMode(10, OUTPUT);
}

void loop() {
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
    irrecv.resume(); // Receive the next value
  }
  
  if (results.value == 0x6785E38A) { // PLAY Button
      digitalWrite(8, LOW);  
  }       
        else
        {
          digitalWrite(8, HIGH);
        }
  
  if (results.value == 0x783D9008) { // PLAY Button
      digitalWrite(9, LOW);  
  }     
        else
        {
          digitalWrite(9, HIGH);
        }
  if (results.value == 0x5F0B42EE) { // PLAY Button
      digitalWrite(10, LOW);  
  }        
        else
        {
          digitalWrite(10, HIGH);
        }
        
  delay(20);
}
