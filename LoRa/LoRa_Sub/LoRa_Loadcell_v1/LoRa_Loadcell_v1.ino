/* 
 Program by Kevin Kwon 2019. 9. 28.

 LoRa communication by Serial 9600bps with Adjusted H/W time
 시리얼통신을 이용한 LoRa통신 프로그램 

 ID Comparing : ID-A01 ~ Z99 (2,574개)
 
 Load Cell data gathering
 IPS (LED ON/OFF)
 Resouce data managing 

 [Information]
 
 Hx711.DOUT - pin #A1
 Hx711.SCK - pin #A0
 
 */

#include <SPI.h>
#include <LoRa.h>
#include "Arduino.h"

String ID = "ID-A01";
String outgoing;
String message;
String rsvData;
String weight;

const int Data4 = 7;
const int Data5 = 8;
const int Data6 = 9;
const int Data7 = 10;

int idstat = 0;                      // ID가 들어왔을때 다음명령가능 활성화

//=============== 무게측정
class Hx711
  {
  public:
  Hx711(uint8_t pin_din, uint8_t pin_slk);
  virtual ~Hx711();
  long value();
  long nomalvalue(byte times = 32);
  void setOffset(long offset);
  void setScale(float scale = 742.f);
  float gram();

  private:
  const uint8_t DOUT;
  const uint8_t SCK;
  long _offset;
  float _scale;
  };

  Hx711 scale(A1, A0);
  Hx711::Hx711(uint8_t pin_dout, uint8_t pin_slk) :
  DOUT(pin_dout), SCK(pin_slk)
  {
    pinMode(SCK, OUTPUT);
    pinMode(DOUT, INPUT);

    digitalWrite(SCK, HIGH);
    delayMicroseconds(100);
    digitalWrite(SCK, LOW);

    nomalvalue();
    this->setOffset(nomalvalue());
    this->setScale();
  }

  Hx711::~Hx711()
  {
  }

  long Hx711::nomalvalue(byte times)
  {
    long sum = 0;
   for (byte i = 0; i < times; i++)
   {
     sum += value();
   }
   return sum / times;
  }

  long Hx711::value()
  {
    byte data[3];
   while (digitalRead(DOUT));
   for (byte j = 3; j--;)
   {
     for (char i = 8; i--;)
     {
       digitalWrite(SCK, HIGH);
       bitWrite(data[j], i, digitalRead(DOUT));
       digitalWrite(SCK, LOW);
     }
   }
    digitalWrite(SCK, HIGH);
   digitalWrite(SCK, LOW);
   data[2] ^= 0x80;
   return ((uint32_t) data[2] << 16) | ((uint32_t) data[1] << 8)
           | (uint32_t) data[0];
  }

void Hx711::setOffset(long offset)
{
 _offset = offset;
}

void Hx711::setScale(float scale)
{
 _scale = scale;
}

float Hx711::gram()
{
 long val = (nomalvalue() - _offset);
 return (float) val / _scale;
}

//=============== 무게측정

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
    Serial.println("Connected :" + ID);
    sendMessage(ID);                   // ID 비교가 완료되어 모듈 ID로 응답
    idstat = 1;                        // 활성화
    delay(10);
   }
   else
   {
    if(idstat == 1)
    {
      if(rsvData == "WI")
      {
        Serial.println("Weight measuring !!! ");
        String weight = String(scale.gram()*2,1);
        Serial.print(weight);
        Serial.println(" g");
        sendMessage(weight);
        delay(1000);
      }
      else if(rsvData == "ID")
      {
        Serial.println(ID);
        sendMessage(ID);
      }
      else if(rsvData == "LO")
      {        
        digitalWrite(Data4, HIGH);                // LED ON
        sendMessage('LED_ON');
        Serial.println("LED_Data4 ON");
      }
      else if(rsvData == "LF")
      {
        digitalWrite(Data4, LOW);                // LED OFF
        sendMessage('LED_OFF');
        Serial.println("LED_Data4 OFF");
      }
      else if(rsvData == "ET")
      {
        Serial.println("ETC .....................");
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
