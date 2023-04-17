void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  float number = random(0, 100);
  Serial.println((String)number);
  delay(1000); // 1 sec delay
}
