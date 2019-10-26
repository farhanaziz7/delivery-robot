#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <SoftwareSerial.h>
#define FIREBASE_HOST "androii-3074d.firebaseio.com"
#define FIREBASE_AUTH "WLYXGraIGSDhSWXIIQELzFmB0hTj1OxFPpfHHZ3Y"

char ssid[] = "z";  //  your network SSID (name)
char pass[] = "12345678";       // your network password
bool jalanB=0, jalanC=0;
bool jalanA=0;
SoftwareSerial komunikasi(D2,D3);
void setup()
{
  komunikasi.begin(9600);
  Serial.begin(115200);
  
  // We start by connecting to a WiFi network
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_OFF);
  delay(1000);
  WiFi.mode(WIFI_STA);
  wifi();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  delay(1000);
}

void loop()
{
  jalanB=0;
  jalanC=0;
  jalanA=0;
  if(WiFi.status() != WL_CONNECTED)
  {
    wifi();
  }
  delay(10);
  
  jalanA=Firebase.getBool("ruangA");
  jalanB=Firebase.getBool("ruangB");
  jalanC=Firebase.getBool("ruangC");
  Serial.println(jalanA);
  Serial.println(jalanB);
  Serial.println(jalanC);
  Serial.println(" ");
  //jalanA = 1
  if (jalanA==1){
    komunikasi.write(2);
    Serial.println("dah didalem");
    
  }
  else if (jalanB==1){
    komunikasi.write(3);
  }
  else if (jalanC==1){
    komunikasi.write(4);
  }

  int baca = komunikasi.read();
  Serial.println(baca);
  
  if(baca==10){
    Firebase.setBool("doneA",false);
  }
  if(baca==11){
    Firebase.setBool("doneB",false);
  }
  if(baca==12){
    Firebase.setBool("doneC",false);
  }
  
  if (baca==5){
    Firebase.setBool("ruangA",false);
    Firebase.setBool("ruangB",false);
    Firebase.setBool("ruangC",false);
  }
  delay(500);
}

void wifi(){
  WiFi.begin(ssid, pass);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
 }