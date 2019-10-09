/* 
 Program by Kevin Kwon 2019. 7. 13.

 LoRa communication by Serial 9600bps with Adjusted H/W time
 시리얼통신을 이용한 LoRa통신 기본 프로그램 

 ID Comparing : ID-A01 ~ Z99 (2,574개)
 
 
 */

#include <SPI.h>
#include <LoRa.h>

String ID = "ID-A01";
String outgoing;
String message;
String rsvData;

const int Data4 = 7;
const int Data5 = 8;
const int Data6 = 9;
const int Data7 = 10;


void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Client Module");

  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }  
  LoRa.setSyncWord(0xBB);           // ranges from 0-0xFF, default 0x34, see API docs
  LoRa.setSpreadingFactor(12); // ranges from 6-12, default 7 see API 
  Serial.println("LoRa init succeeded.");

//  clientInfo();
  pinMode(Data4, OUTPUT);
}

void loop() {
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("Received: ");

    // read packet
    while (LoRa.available()) {
      rsvData += (char)LoRa.read();
    }
    Serial.print(rsvData);
    Serial.print(" ' with RSSI ");
    Serial.println(LoRa.packetRssi()); // 수신측에 넣어야 정상동작

// ID check
    chkProcess();    
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

void chkProcess()
{
   if (rsvData.compareTo(ID) == 0){
    digitalWrite(Data4, HIGH);   // ID가 호출되었으니 LED ON
    Serial.println("Connected :" + ID);
    sendMessage(ID);  // ID 비교가 완료되어 모듈 ID로 응답
    delay(10);
  }
  else
  {
    digitalWrite(Data4, LOW); // 아니면 LED OFF
    delay(1000);     // ID가 다르면 1초 동안 정지하여 다시 호출 대기    
  } 
    rsvData = "";
}
