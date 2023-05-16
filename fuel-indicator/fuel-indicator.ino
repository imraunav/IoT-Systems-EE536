#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
// #define LED_BUILTIN D0
int ledState = LOW;

unsigned long previousMillis = 0;
unsigned long currentMillis = millis();
const long interval = 50;

#ifndef STASSID
#define STASSID "Raunav's 7x"
#define STAPSK  "s64j9d5u"
#endif

// #define IP_ADDRESS "raunav-rpi.local"
// String MY_HOSTNAME = "fuel-indicator";
#define PORT 8885


WiFiUDP udp;
char packetBuffer[UDP_TX_PACKET_MAX_SIZE + 1];

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  // WiFi.hostname(MY_HOSTNAME.c_str());
  WiFi.begin(STASSID, STAPSK);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(500);
  }
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());
  Serial.printf("UDP server on port %d\n", PORT);
  udp.begin(PORT);
  Serial.printf("Hostname: %s\n", WiFi.hostname().c_str());
}

void blink_led(){
  // adapted from blink without delay 
  Serial.println("Blinking"); // debug
  currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    if (ledState == LOW) {
      ledState = HIGH;  // Note that this switches the LED *off*
    } else {
      ledState = LOW;  // Note that this switches the LED *on*
    }
    digitalWrite(LED_BUILTIN, ledState);
  }
}

void steady_led(){
  Serial.println("Steady"); //debug 
  digitalWrite(LED_BUILTIN, LOW);
  // delay(100);
}
void loop() {
  // put your main code here, to run repeatedly:
  int packetSize = udp.parsePacket();
  if(packetSize!=0){
    Serial.printf("Received packet of size %d from %s:%d\n    (to %s:%d, free heap = %d B)\n",
                  packetSize,
                  udp.remoteIP().toString().c_str(), udp.remotePort(),
                  udp.destinationIP().toString().c_str(), udp.localPort(),
                  ESP.getFreeHeap());
    int n = udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
    packetBuffer[n] = 0;
    Serial.println("Contents:");
    Serial.println(packetBuffer);

    if(String(packetBuffer).toFloat()>20){
      steady_led();
    }
    else{
      blink_led();
    }
  }
    // udp.beginPacket(IP_ADDRESS, PORT);
    // int n = udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
    // packetBuffer[n] = 0;
    // Serial.println("Contents:");
    // Serial.println(packetBuffer); 
    // udp.endPacket();
}
