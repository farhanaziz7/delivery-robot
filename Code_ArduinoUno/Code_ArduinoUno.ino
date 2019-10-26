//Motor
#define m1 3
#define m2 5
#define m3 9
#define m4 10
#define buzzer 13

#include <SoftwareSerial.h>
SoftwareSerial komunikasi(6,7); //RX TX

bool nganterinA=false;
bool nganterinB=false;
bool nganterinC=false;

bool idlee=true;

int andro=0;

int lbelakang,rbelakang,lsensor,lstop,rsensor,rstop,btnA,btnB,btnC;
int arrow = 0;

void setup(){
  Serial.begin(115200);
  komunikasi.begin(9600);
  
  //OUTPUT
  pinMode(m1,OUTPUT);
  pinMode(m2,OUTPUT);
  pinMode(m3,OUTPUT);
  pinMode(m4,OUTPUT);
  pinMode(buzzer,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(12,OUTPUT);
  
  //INPUT
  pinMode(A0,INPUT);
  pinMode(A1,INPUT);
  pinMode(A2,INPUT);
  pinMode(A3,INPUT);
  pinMode(A4,INPUT);
  pinMode(A5,INPUT);
  pinMode(2,INPUT);
  pinMode(4,INPUT); 
}

void loop() {

//DEKLARASI
  // pembacaan sensor IR Track
  lsensor=digitalRead(A2);
  rsensor=digitalRead(A0);
  rbelakang=digitalRead(A1);
  lbelakang=digitalRead(4);
  lstop=digitalRead(A3);
  
  // Button
  btnA=digitalRead(A4);
  btnB=digitalRead(A5);
  btnC=digitalRead(2);
  
  if(idlee){
    modeIdle();
  }
  
  if(komunikasi.available()>0){
    andro = komunikasi.read();
    Serial.print("andalem: ");
    Serial.println(andro);
    
  }
    Serial.println(andro);
      if(btnA==HIGH || andro == 2){
        idlee=false;
        nganterinA=true;
      }
      else if(btnB==HIGH || andro == 3){
        idlee=false;
        nganterinB=true;
      }
      else if(btnC==HIGH || andro == 4){
        idlee=false;
        nganterinC=true;
      }    
//------------------------------------------------------------------------
        if(nganterinA){
          Serial.println("A");
          modeBtnA();
            jalan();
            if(lstop==LOW){
               motorOut(0,0,1);
               komunikasi.write(10);
               nganterinA=false;
               
              while(!nganterinC){
                btnC=digitalRead(2);
                andro = komunikasi.read();
                Serial.println(andro);
                WaitingBtnC();
                if(btnC==HIGH || andro==4 ){
                  nganterinC=true;
                }
              }
             }
        }
//------------------------------------------------------------------------
        if(nganterinB){
          Serial.println("B");
          modeBtnB();
             jalan();
             if((lstop==LOW) && (lsensor==LOW) && (rsensor==LOW)){
               motorOut(0,0,1);
               komunikasi.write(11);
               nganterinB=false;
               
              while(!nganterinC){
                btnC=digitalRead(2);
                andro = komunikasi.read();
                WaitingBtnC();
                if(btnC==HIGH || andro==4){
                  nganterinC=true;
                }
              }
             }
        }
//------------------------------------------------------------------------
        if(nganterinC){
          Serial.println("C");
          modeBtnC();
          mundur();
          
          if((lbelakang==LOW)&&(rbelakang==LOW)){
            motorOut(0,0,2);
            komunikasi.write(12);
            nganterinC=false;
            idlee=true;
          }
        }
}



//FUNGSI
void buzzerStart(){
  digitalWrite(buzzer, HIGH);
    delay(500);
    digitalWrite(buzzer, LOW);
    delay(500);
    digitalWrite(buzzer, HIGH);
    delay(500);
    digitalWrite(buzzer, LOW);
    delay(200);
}
void modeIdle(){
    digitalWrite(8,HIGH);
    digitalWrite(11,HIGH);
    digitalWrite(12,HIGH);
}

void modeBtnA(){
    digitalWrite(8,LOW);
    digitalWrite(11,LOW);
    digitalWrite(12,HIGH);
    nganterinB=false;
    nganterinC=false;
}

void modeBtnB(){
    digitalWrite(8,LOW);
    digitalWrite(11,HIGH);
    digitalWrite(12,LOW);
    nganterinA=false;
    nganterinC=false;
}

void modeBtnC(){
    digitalWrite(8,HIGH);
    digitalWrite(11,LOW);
    digitalWrite(12,LOW);
    nganterinA=false;
    nganterinB=false;
    digitalWrite(buzzer, LOW);
}

void WaitingBtnC(){
    digitalWrite(11,LOW);
    digitalWrite(12,LOW);
    digitalWrite(8,HIGH);
    digitalWrite(buzzer, HIGH);
    delay(300);
    digitalWrite(buzzer, LOW);
    digitalWrite(8,LOW);
    delay(300);
}

void jalan(){
  
  if((lsensor==HIGH)&&(rsensor==HIGH))
    {
    //maju
    motorOut(150,250,2);
    }
  else if((lsensor==LOW)&&(rsensor== HIGH))
    {
    // belok kanan
    motorOut(10,0,1);
    motorOut(0,255,2);
    }
  else if((lsensor== HIGH)&&(rsensor== LOW))
    {
    //belok kiri
    motorOut(0,10,1);
    motorOut(150,0,2);
    }
  else if((lsensor==LOW)&&(rsensor==LOW))
    {
    //berhenti
    motorOut(0,0,2);
    }
}

void mundur(){
  //logika robot menggunakan prinsip  AND
  if((lbelakang==HIGH)&&(rbelakang==HIGH))
    {
    //maju
    motorOut(210,210,1);
    }
  else if((lbelakang==LOW)&&(rbelakang== HIGH))
    {
    // belok kanan
    motorOut(0,10,2);
    motorOut(210,0,1);
    }
  else if((lbelakang== HIGH)&&(rbelakang== LOW))
    {
    //belok kiri
    motorOut(10,0,2);
    motorOut(0,230,1);
    }
    //fungsi stop & mundur
}

void motorOut(unsigned char lpwm, unsigned char rpwm, int arrow){
//arrow = 1 mundur, 2 maju,
if(arrow==1){
  digitalWrite(m3,HIGH);
  digitalWrite(m1,LOW);
  analogWrite(m4,255-lpwm);
  analogWrite(m2,rpwm);
}
else if (arrow==2){
  digitalWrite(m3,LOW);
  digitalWrite(m1,HIGH);
  analogWrite(m4,lpwm);
  analogWrite(m2,255-rpwm);
  }
}