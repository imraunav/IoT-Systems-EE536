#include<ESP8266WiFi.h>
#include<WiFiClient.h>
#include<PubSubClient.h>

//#define MAXATTEMPT 5

/* ---Connection configuration--- */
IPAddress broker(10,7,10,194); //(192,168,1,111);
//IPAddress staticip(10,8,11,29);  // can uncomment this line to make the ip of the node MCU static
//IPAddress gateway(10,8,0,1);
//IPAddress subnet(255,255,0,0);
const char *ssid = "IITMandi_WiFi"; 
const char *pass = "wifi@iit"; 


WiFiClient wifiesp_client;
PubSubClient MQTTclient(wifiesp_client);
char tempData[10]; 

void callback(char* topic, byte* payload, unsigned int length) {
 
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
 
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
 
  Serial.println();
  Serial.println("-----------------------");
 
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Entered Set-up");
  //WiFi.config(staticip,gateway,subnet,gateway);  // uncomment this in case you want a static ip
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid,pass);
  while(WiFi.status()!=WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.print("Connected to access-point : ");
  Serial.println(ssid);
  Serial.print("IP address : ");
  Serial.println(WiFi.localIP());
  MQTTclient.setServer(broker, 1883); // broker server	
  MQTTclient.setCallback(callback); //setting the callback 
  Serial.print("Attempting MQTT connection: ");
  
  // while(MQTTclient.connect())
  while(!MQTTclient.connected()){
    if(MQTTclient.connect("House1")){
      Serial.println("Connected to MQTT broker");
      break;
    }
    else{
      Serial.print("failed, return code=");
      Serial.println(MQTTclient.state());
      delay(5000);
    }
  }

  // MQTTclient.publish("network/peakperiod", "Hello from House1");
  MQTTclient.subscribe("house-network/peakperiod");
}

void loop() {
  // put your main code here, to run repeatedly:
  // String str=String(random(0,25));
  // str.toCharArray(tempData,10);
  // Serial.println(tempData);
  // MQTTclient.publish("/",tempData,true);
  // delay(5000);
  MQTTclient.loop();
}
