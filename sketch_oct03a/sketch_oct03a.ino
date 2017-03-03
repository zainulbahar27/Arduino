int val = 0;

void setup() {
  Serial.begin(9600);
  pinMode(9, OUTPUT);
}

void loop() {
  val = analogRead(A0);
  analogWrite(9,val/4);
  Serial.print("PWM Value =');
  Serial.println(val,DEC);
  delay(500);
}
