
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#define LED_BUILTIN D0

#ifndef STASSID
#define STASSID "Raunav's 7x"
#define STAPSK  "s64j9d5u"
#endif

#define IP_ADDRESS "192.168.27.186" //RPi's address
#define PORT1 8888 //fuel 1
#define PORT2 8889 //fuel 2

// unsigned int localPort = 8888;      // local port to listen on

WiFiUDP Udp0; // Receive refuel
WiFiUDP Udp1; // Broadcast fuel1
WiFiUDP Udp2; // Broadcast fuel2
char packetBuffer[UDP_TX_PACKET_MAX_SIZE + 1];


// This is an error handling flag
// bool flag = 0;

// char  ReplyBuffer[] = "acknowledged\r\n"; 
float petrol1(int t){
  float reading = -0.23*t+100;
  if(reading<0){
    return 0;
  }
  else{
    return reading;
  }
}
float petrol2(int t){
  float reading =  -0.29*t+100;
  if(reading<0){
    return 0;
  }
  else{
    return reading;
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(STASSID, STAPSK);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(500);
  }
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());
  Serial.printf("UDP server on port %d\n", PORT1);
  Serial.printf("UDP server on port %d\n", PORT2);
  Udp1.begin(PORT1);
  Udp2.begin(PORT2);

}

void loop() {
  // put your main code here, to run repeatedly:
  //if(Serial.available()!=0){
  // flag = 1;
  // String message = Serial.readString();
  //float message = Serial.parseFloat();
  // char buffer[30];
  // message.toCharArray(buffer, message.length());
  for(int i=0;i<=400;i++){
    
    float message1 = petrol1(i);
    float message2 = petrol2(i);

    Udp1.beginPacket(IP_ADDRESS, PORT1);
    // Serial.println(message);
    Udp1.println(message1);
    Udp1.endPacket();

    Udp2.beginPacket(IP_ADDRESS, PORT2);
    // Serial.println(message);
    Udp2.println(message2);
    Udp2.endPacket();

    delay(20);
  }
    // delay(1000);
    // stream s;
    // s.flush();
}
