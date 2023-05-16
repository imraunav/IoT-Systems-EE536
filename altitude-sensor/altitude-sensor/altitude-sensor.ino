int altitude(int t){
  if(t<100){
    return t;
  }
  else if(t>300){
    return -t+400;
  }
  else{
  return 100;
  }
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  // flight on ground
  for(int i=0; i<3; i++){
    Serial.println(0);
    delay(500);
  }
  // flight begins
  for(int i=0;i<=400;i++)
  {
    float alt=altitude(i);
    Serial.println(alt);
    delay(100);
  }
  
}
