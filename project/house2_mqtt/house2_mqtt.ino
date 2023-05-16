#include<ESP8266WiFi.h>
#include<WiFiClient.h>
#include<PubSubClient.h>

unsigned long previousMillis = 0;
const long interval = 5000;
bool acState = 1;

const char *ssid = "Raunav's 7x"; 
const char *pass = "s64j9d5u"; 
const char *broker = "raunav-rpi.local"; // hostname of the broker
const int broker_port = 1883;
const char *client_name = "house2";

String sub_topic = "house-network/peakperiod";
const char *pub_topic = "house-network/house2-power";

WiFiClient wifi_esp;
PubSubClient mqtt_client(wifi_esp);
// PubSubClient mqtt_simulation(wifi_esp);


// global variable to track the status of the peak period
int peakPeriod = 0;
int simulation = 0;

// callback function to be executed when called
void callback(char* topic, byte* payload, unsigned int length){
  
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  
  Serial.print("Message:");

  if(strcmp(topic,"house-network/peakperiod")==0){
    
    peakPeriod = (char)payload[0] - '0'; // ascii math to convert char to string
    Serial.println(peakPeriod);
    Serial.println();
  }
  if(strcmp(topic, "house-simulation")==0){
    simulation = (char)payload[0] - '0'; // ascii math to convert char to string
    if(simulation==1){
      Serial.println("Starting simulation");
    }
  }
  Serial.println("-----------------------");
}

// void callback_simulation(char* topic, byte* payload, unsigned int length){
//   Serial.print("Message arrived in topic: ");
//   Serial.println(topic);
 
//   // Serial.print("Message:");
//   // for (int i = 0; i < length; i++) {
//   //   Serial.print((char)payload[i]);
//   // }
//   simulation = (char)payload[0] - '0'; // ascii math to convert char to string
//   if(simulation==1){
//     Serial.println("Starting simulation");
//   }
//   else{
//     Serial.println("Not-here!");
//   }
//   Serial.println();
//   Serial.println("-----------------------");
// }

int hourCount = 0;
int washperDay = 1; // no. of washes per day
int deferWash = 0;
int washingMachine(){
  if(washperDay > 0){
    if(peakPeriod == 0 || deferWash == 23){
      washperDay--;
      deferWash == 0;
      return 500; // washing machine is on
    }
    else{
      deferWash++; 
    }
  }
  return 0; // washing machine is off
}
int teleVision(){
  int state = random(0,10); // to determine if tv is on or not 
  if(state%2 == 0){
    return 0; // tv is off
  }
  else{
    return 50; // tv is on
  }
}

int airConditioner(){
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
  Serial.println("Setting up...");

  // setting up the wifi
  WiFi.mode(WIFI_STA); // esp setup as wifistation
  WiFi.begin(ssid, pass);
  while(WiFi.status()!=WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }
  Serial.print("Connected to access point : ");
  Serial.println(ssid);
  Serial.print("IP address : ");
  Serial.println(WiFi.localIP());

  // setting up mqtt
  mqtt_client.setServer(broker, broker_port); // setting up the mqtt broker connection
  mqtt_client.setCallback(callback); // setting callback for action to happen once message is published to topic

  // mqtt_simulation.setServer(broker, broker_port);
  // mqtt_simulation.setCallback(callback_simulation);

  while(!mqtt_client.connected()){
    if(mqtt_client.connect(client_name)){
      Serial.println("Connected to MQTT broker");
      // Serial.print(mqtt_client.localIP());
      break;
    }
    else{
      Serial.print("failed, return code=");
      Serial.println(mqtt_client.state());
      delay(5000);
    }
  // subcribe to topicto know the peak-period
  }
  mqtt_client.subscribe("house-network/peakperiod");
  mqtt_client.subscribe("house-simulation");
}
//   while(!mqtt_simulation.connected()){
//     if(mqtt_simulation.connect(client_name)){
//       Serial.println("Connected to MQTT broker");
//       // Serial.print(mqtt_client.localIP());
//       break;
//     }
//     else{
//       Serial.print("failed, return code=");
//       Serial.println(mqtt_simulation.state());
//       delay(5000);
//     }
//   }
// }

void loop() {
  // put your main code here, to run repeatedly:
    mqtt_client.loop();
    //mqtt_simulation.loop();
    if(simulation>0){
      // Serial.println("HaHa");
      int w = washingMachine();
      int t = teleVision();
      int ac = airConditioner();
      int total_power = w + t + ac;

      char cstr[16];
      itoa(total_power, cstr, 10);
      mqtt_client.publish(pub_topic, cstr);
      itoa(peakPeriod, cstr, 10);
      mqtt_client.publish("house-network/house-2-peakPeriod-declare", cstr);
      hourCount++;
      delay(1000);
  }
  if(hourCount>24){ // reset the day
    simulation = 0;
    hourCount = 0;
    washperDay = 1; 
  }
}
