
#include <DHT.h>

int pin = A1;  // 핀설정
DHT11 dht11(pin); 

void setup() {
  Serial.begin(9600);
}

void loop() {
  int i;
  float humi, temp;
  if((i = dht11.read(humi, temp)) == 0) {  // 온도, 습도 값을 읽어오면
    Serial.print("humidity:");          // ‘시리얼 플로터’ 사용위해 이부분 주석 필요
    Serial.println(humi);                  // 습도값 출력
    Serial.print("temperature:");       // ‘시리얼 플로터’ 사용위해 이부분 주석 필요
    Serial.println(temp);                  // 온도값 출력 
  } 
  else{ 
    Serial.print("Error:");                    
    Serial.print(i);                          
  }  
  delay(1000);
}  
 
