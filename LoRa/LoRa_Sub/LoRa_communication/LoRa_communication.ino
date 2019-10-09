/* 
 Program by Kevin Kwon 2019. 9. 28.

 LoRa communication by Serial 9600bps with Adjusted H/W time
 시리얼통신을 이용한 LoRa통신 기본 프로그램 

 ID Comparing : ID-A01 ~ Z99 (2,574개)
 
 ID call and next process function 
 
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

int keeptime = 100;                  // ID가 호출되었을때 10초간 활성화
int idstat = 0;                      // ID가 들어왔을때 Next 명령을 받는 시간

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Comunication Module : " + ID);

  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }  
  LoRa.setSyncWord(0xBB);               // ranges from 0-0xFF, default 0x34, see API docs
  LoRa.setSpreadingFactor(12);          // ranges from 6-12, default 7 see API 
  Serial.println("LoRa init succeeded.");

//  clientInfo();
  sendMessage(ID);                      // 초기, Reset시 ID 전송
  pinMode(Data4, OUTPUT);
}

void loop() {
                                        // try to parse packet
  int packetSize = LoRa.parsePacket();
  if(packetSize) 
  {                     // received a packet
    Serial.print("Received: ");         // read packet
    while (LoRa.available()) {
      rsvData += (char)LoRa.read();
      }
    Serial.print(rsvData);
    Serial.print(" ' with RSSI ");
    Serial.println(LoRa.packetRssi()); // 수신측에 넣어야 정상동작
    chkProcess();                      // ID check
  }

  if(idstat >= 1)
  {
    idstat--;
    Serial.println(idstat);    
    delay(100);                         // Time = ? 
  }
//  else
//  {    
//      digitalWrite(Data4, LOW);          // ID가 미선택이면 LED OFF
//  }
    
  // Send
  if (Serial.available()>0) {
    while (Serial.available()) {
      message += (char)Serial.read();
      delayMicroseconds(1050);         // Serial 동작 H/W와 Arduino 동작속도 차이보정
    }        
    sendMessage(message);
    Serial.print(message);
  }
  message = "";    
}

void sendMessage(String outgoing) {    // LoRa 송신
  LoRa.beginPacket();                  // start packet
  LoRa.print(outgoing);                // add destination address
  LoRa.endPacket();                    // finish packet and send it
}

void chkProcess()                      // ID 확인 및 전송
{
   if (rsvData.compareTo(ID) == 0)
   {
    digitalWrite(Data4, HIGH);         // ID가 호출되었으니 LED ON
    Serial.println("Connected :" + ID);
    sendMessage(ID);                   // ID 비교가 완료되어 모듈 ID로 응답
    idstat = keeptime;                        // Next code 받을수 있는 준비시간
    delay(10);
   }
   else
   {
    if(idstat >= 1)
    {
      if(rsvData == "WI")
      {
        Serial.println("Weight measuring !!! ");
        idstat = keeptime;                        // Next code 받을수 있는 준비시간
      }
      else if(rsvData == "ET")
      {
        Serial.println("ETC .....................");
        idstat = keeptime;                        // Next code 받을수 있는 준비시간
      }
      else
      {
        Serial.println("NA" + rsvData);
        idstat = 0;                    //Next code 이외는 프로세스 중단
      }      
    }
  }   
  rsvData = "";                        // 받은 Data는 삭제
}
