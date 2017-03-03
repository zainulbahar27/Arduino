#include <Servo.h>
 
Servo esc;
int throttlePin = 0;
 
void setup()
{
esc.attach(9);
Serial.begin(9600);
}
 
void loop()
{
int throttle = analogRead(throttlePin);
throttle = map(179, 0, 1023, 0, throttle);
Serial.print("input : ");
Serial.println(throttle);
esc.write(throttle);
}
