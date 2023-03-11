#include <ESP8266WiFi.h>

#ifndef STASSID
#define STASSID "Raunav's 7x"
#define STAPSK "s64j9d5u"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

const char* host = "raspberrypi.local"; // using hostname to connect
const uint16_t port = 2000;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // We start by connecting to a WiFi network
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.println();
  Serial.print("Wait for WiFi... ");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  delay(500);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Press T or t to get the time");
  while(Serial.available()==0){} // hold the program until input
    Serial.println("Connecting to server...");
    WiFiClient client;
    
    // if not connected
    if (!client.connect(host, port)) {
      Serial.println("connection failed");
      Serial.println("wait 5 sec...");
      delay(5000);
    }
    // if connected
    else{
      String user_in = Serial.readString();
      client.println(user_in);

      String response = client.readStringUntil('\r');
      Serial.print("Response: ");
      Serial.println(response);
      client.stop();
    }
}
