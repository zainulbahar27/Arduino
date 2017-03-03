int val =0;
void setup() {
  Serial.begin(9600);
  pinMode(10,OUTPUT);
  pinMode(5,OUTPUT);
  // put your setup code here, to run once:

}

void loop() {
 val = analogRead(A0);
 int pwm=val/4;
 Serial.print("PWM Value =");
 Serial.println(pwm,DEC);
 if (pwm<=0)
 {
  digitalWrite(10,LOW);
  digitalWrite(5,LOW);
  delay(1000); 
 }
 else if(110>pwm)
 {
   digitalWrite(10,HIGH);
  digitalWrite(5,LOW);
  delay(1000);
 }
 else
 {
  digitalWrite(10,LOW);
  digitalWrite(5,HIGH);
  delay(1000);
 }
 delay(500);
}
