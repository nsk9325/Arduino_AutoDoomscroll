#include <WiFiS3.h>  // UNO R4 WiFi용 라이브러리

const char* ssid = "makerspace hall";
const char* password = "makerspace1010!";
const char* serverIP = "192.168.50.110"; // 마스터 보드의 IP
const int serverPort = 80;

const int sensorPin = A0;
const int TH = 500;
const int HYSTERESIS = 60;
bool above = false;
int sensorData;
bool state;


WiFiClient client;

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected!");
}

void loop() {
  sensorData = analogRead(sensorPin);  // 센서값 예시
  Serial.println(sensorData);
  setprintstate();

  if (client.connect(serverIP, serverPort)) {
    // HTTP GET 요청 전송
    client.print("GET /data?value=");
    //client.print(sensorData);
    printtoclient2();
    client.println(" HTTP/1.1");
    client.print("Host: ");
    client.println(serverIP);
    client.println("Connection: close");
    client.println();


    client.stop();
  } else {
    Serial.println("Connection failed");
  }

  delay(150);  // 주기적 전송
}

void printtoclient(){
  if(!above && sensorData >= TH){
    client.print(1);
    Serial.println(">> SENT: A");
    above = true;
  }else{
    client.print(0);
  }
  if(above && sensorData <= TH - HYSTERESIS){
    above = false;
  }
}

void printtoclient2(){
  if(state == true){
    client.print(1);
    Serial.println(">> SENT: A");
  }else{
    client.print(0);
  }
}

void setprintstate(){
  if(!above && sensorData >= TH){
    state = true;
    above = true;
  }else{
    state = false;
  }
  if(above && sensorData <= TH - HYSTERESIS){
    above = false;
  }
}