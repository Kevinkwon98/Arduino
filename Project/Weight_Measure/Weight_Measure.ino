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

void setup(){
  Serial.begin(9600);
  HX711init();
}

void loop(){
  Serial.print(getGram(4),1); // 32번 읽어서 평균 // 소숫점이하 자릿수=1
  Serial.println(" g");
  delay(200);
}
