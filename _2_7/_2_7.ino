int button=0;
void setup() {
  // put your setup code here, to run once:

pinMode(13,OUTPUT);
pinMode(7,INPUT);
}

void loop() {
  // put your main code here, to run repeatedly: 
  button = digitalRead(7);
  if (button == HIGH){
  digitalWrite(13,HIGH);
  }
  else{
    digitalWrite(13,LOW);
  }
}
