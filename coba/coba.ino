void setup() {                
// Turn the Serial Protocol ON
  Serial.begin(9600);
}
void loop() {
  // put your main code here, to run repeatedly:
 Serial.print("A7 : ");
 Serial.println(analogRead(A7));
 delay(1000);
 Serial.print("A3 : ");
 Serial.println(analogRead(A3));
 delay(1000);
 Serial.print("A2 : ");
 Serial.println(analogRead(A2));
 delay(1000);
 Serial.print("A1 : ");
 Serial.println(analogRead(A1));
 delay(1000);
 Serial.print("A0 : ");
 Serial.println(analogRead(A0));
 delay(2000);
}
