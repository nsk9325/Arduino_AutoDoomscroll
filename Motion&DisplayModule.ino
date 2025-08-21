#include <WiFiS3.h>
#include <Servo.h>

const char* ssid = "makerspace hall";
const char* password = "makerspace1010!";

WiFiServer server(80);

Servo xy;
Servo z;

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();
  xy.attach(5);
  z.attach(6);

  xy.write(0);
  z.write(-60);
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    String request = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        request += c;
        if (request.endsWith("\r\n\r\n")) break;
      }
    }

    // 요청에서 데이터 파싱
    int valueIndex = request.indexOf("value=");
    if (valueIndex != -1) {
      String valueStr = request.substring(valueIndex + 6, request.indexOf(" ", valueIndex));
      int sensorValue = valueStr.toInt();
      Serial.print("Received value: ");
      Serial.println(sensorValue);

      if(sensorValue == 1){
        Serial.println("모터 작동");
        z.write(45);   delay(400);
        xy.write(-90);  delay(400);
        z.write(-45);  
        xy.write(90); delay(400);
      }


    }

    // 간단한 HTTP 응답
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/plain");
    client.println("Connection: close");
    client.println();
    client.println("OK");

    delay(10);
    client.stop();
  }
}