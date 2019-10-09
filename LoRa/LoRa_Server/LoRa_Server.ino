/* 
 Program by Kevin Kwon 2019. 7. 13.

 LoRa communication by Serial 9600bps with Adjusted H/W time
 시리얼통신을 이용한 LoRa통신 기본 프로그램  
 
 */

#include <SPI.h>
#include <LoRa.h>

String outgoing;
String message;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Server Module");

  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }  
  LoRa.setSyncWord(0xBB);           // ranges from 0-0xFF, default 0x34, see API docs
  LoRa.setSpreadingFactor(12); // ranges from 6-12, default 7 see API 
  Serial.println("LoRa init succeeded.");
}

void loop() {
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("Received: ");

    // read packet
    while (LoRa.available()) {
      Serial.print((char)LoRa.read());
    }
    Serial.print(" ' with RSSI ");
    Serial.println(LoRa.packetRssi()); // 수신측에 넣어야 정상동작
  }

  // Send
  if (Serial.available()>0) {
    while (Serial.available()) {
      message += (char)Serial.read();
      delayMicroseconds(1050);   // Serial 동작 H/W와 Arduino 동작속도 차이보정
    }        
    sendMessage(message);
    Serial.print(message);
  }
  message = "";    
}

void sendMessage(String outgoing) {
  LoRa.beginPacket();                   // start packet
  LoRa.print(outgoing);              // add destination address
  LoRa.endPacket();                     // finish packet and send it
}
