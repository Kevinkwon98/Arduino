#include <DHT.h>
#include <SoftwareSerial.h>
const int UltrasonicWave = 0;
const int Temperature = 1;
const int Humidity = 2;
const int SensorID = 0;
int blueTx=2;   //Tx (보내는핀 설정)at
int blueRx=3;   //Rx (받는핀 설정)

#define DHTPIN A1
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

SoftwareSerial mySerial(blueTx , blueRx);  //시리얼 통신을 위한 객체선언
char dataValue;

float temp;
float humi;

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600); //블루투스 시리얼
  dht.begin();
}

void loop() {    
  
  if (mySerial.available()) {    
    dataValue = mySerial.read();
    Serial.println(String(dataValue));  //블루투스측 내용을 시리얼모니터에 출력
  }
  humi = dht.readHumidity();
  temp = dht.readTemperature();

  mySerial.println(String(Temperature) + '|' + String(SensorID) + '|' + String(temp));
  Serial.println(String(temp) + " C");
  mySerial.println(String(Humidity) + '|' + String(SensorID) + '|' + String(humi));
  Serial.println(String(humi) + " %");

  delay(5000);
}
