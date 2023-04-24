unsigned long previousMillis = 0;
unsigned long currentMillis = millis();
const long interval = 50;
bool ledState = 1;

void blink(){
  // Serial.println("Blinking");
  // digitalWrite(LED_BUILTIN, HIGH);
  // delay(100);
  // digitalWrite(LED_BUILTIN, LOW);
  // delay(100);
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

void steady(){
  Serial.println("Steady");
  digitalWrite(LED_BUILTIN, HIGH);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()!=0){
    float alti = Serial.parseFloat();
    Serial.println(alti);
    if(alti<100){
      blink();
    }
    else{
      steady();
    }
    // delay(100);
  }
}
