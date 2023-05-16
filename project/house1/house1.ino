
#include <ESP8266WiFi.h>

#ifndef STASSID
#define STASSID "Raunav's 7x"
#define STAPSK "s64j9d5u"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

const char* host = "raunav-rpi.local"; // using hostname to connect
const uint16_t port = 2000; // tcp- port number 

unsigned long previousMillis = 0;
const long interval = 5000;
bool acState = 1;

String peakPeriod = "0";

WiFiClient client;
int washingMachine(String peakPeriod){
  if(peakPeriod == "0"){
    return 500; // washing machine is on
  }
  return 0; // washing machine is off
}

int teleVision(String peakPeriod){
  int state = random(0,10); // to determine if tv is on or not 
  if(state%2 == 0){
    return 0; // tv is off
  }
  else{
    return 50; // tv is on
  }
}

int airConditioner(String peakPeriod){
  unsigned long currentMillis = millis();
  if(currentMillis-previousMillis > interval){ // turn on ac for some time and turn it off for the next hour
    previousMillis = currentMillis;
    acState = !acState; // change in acState
  }
  return acState*300;
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
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

  while(!client.connect(host, port)) { //connecting to server; must connect to server
      Serial.println("connection failed");
      Serial.println("wait 5 sec...");
      delay(5000);
    }
  Serial.println("Connected to raspberrypi!");
  Serial.println(client.remoteIP());
  Serial.println("---------------");
  // delay(500);
}

// bool peakPeriod = 0;
void loop() {
  // put your main code here, to run repeatedly:
  // if(Serial.available()!=0){
  //   peakPeriod = Serial.parseInt();
  //   // Serial.print("J");
  // }
  int w = washingMachine(peakPeriod);
  int t = teleVision(peakPeriod);
  int ac = airConditioner(peakPeriod);

  int total_power = w + t + ac;
  client.print(total_power);
  // peakPeriod = client.readStringUntil('\r');
  if(client.available()!=0){
    if(client.read() == true){
      peakPeriod = "1";
    }
    else{
      peakPeriod = "0";
    }
  }

  // if(Serial.available()!=0){
  if(true){
    Serial.read();
    Serial.read();

    Serial.print("Washing machine power: ");
    Serial.println(w);
    Serial.print("Television power: ");
    Serial.println(t);
    Serial.print("AC power: ");
    Serial.println(ac);
    
    Serial.print("Total power this hour: ");
    Serial.println(total_power);
    Serial.print("PeakPeriod: ");
    Serial.println(peakPeriod);
    Serial.println("---------------");
  }

  delay(1000);
}
