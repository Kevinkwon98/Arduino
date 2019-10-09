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
#define U08 unsigned char
#define U32 unsigned long

#define SCK A0
#define DIN A1

long  offset=0;
float scale=742.0;

U32 getValue(){
  U08 i,j,data[3];
  while(digitalRead(DIN)); // wait for Ready

  for(j=3; j--;){
    for(i=8; i--;){ digitalWrite(SCK,1); bitWrite(data[j],i,digitalRead(DIN)); digitalWrite(SCK,0); }
  }

  digitalWrite(SCK,1); digitalWrite(SCK,0);   // 25클럭 // Next Conversion：CH.A,Gain: 128
  //digitalWrite(SCK,1); digitalWrite(SCK,0); // 26클럭 // Next Conversion：CH.B,Gain: 32
  //digitalWrite(SCK,1); digitalWrite(SCK,0); // 27클럭 // Next Conversion：CH.B,Gain: 64

  data[2]^=0x80;

  return((U32)data[2]<<16)|((U32)data[1]<<8)|(U32)data[0];
}

void setOffset(){ offset=getValue(); }
void setScale(float s){ scale=s; }

float getGram(U08 times){
  long val,sum=0; U08 i;
  for(i=0; i<times; i++)sum+=getValue();
  sum/=times;
  val=sum-offset;
  return(float)val/scale;
}

void HX711init(){
  pinMode(DIN,INPUT);
  pinMode(SCK,OUTPUT);
  digitalWrite(SCK,1); delayMicroseconds(100); digitalWrite(SCK,0); // 60us 이상 POWER DOWN 후 Nomal Mode
  setOffset(); setOffset(); // 2번째 변환 결과로 Offset 적용
}

//=============== 무게측정

void setup() {
  Serial.begin(9600);
  HX711init();
  
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
        String weight = String(getGram(4),1); // 4번 읽어서 평균 소숫점이하 자릿수=1
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
