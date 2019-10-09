#include <SoftwareSerial.h>

const int UltrasonicWave = 0;
const int SensorID = 0;
int blueTx=2;   //Tx (보내는핀 설정)at
int blueRx=3;   //Rx (받는핀 설정)
int echoPin = 9;
int triggerPin = 10;
int duration;
int currentDistance;
SoftwareSerial mySerial(blueTx , blueRx);  //시리얼 통신을 위한 객체선언
char dataValue;
bool isFirst = true;

void setup() {
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);  
  Serial.begin(9600);
  mySerial.begin(9600); //블루투스 시리얼
}

void loop() {    
  if (mySerial.available()) {    
    dataValue = mySerial.read();
    Serial.println(String(dataValue));  //블루투스측 내용을 시리얼모니터에 출력
  }  
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  currentDistance = pulseIn(echoPin, HIGH, 29000) / 58;  
  Serial.print(currentDistance);
  Serial.println(" cm");   
  if(currentDistance > 0 && currentDistance < 300) {
      mySerial.println(String(UltrasonicWave) + '|' + String(SensorID) + '|' + String(currentDistance));  //초음파센서로부터 받은 정보를 블루추스 측에 WRITE 형식: 센서고유값 0 + 구분자(|) + 센서정보값
    }    
  delay(250);
}
