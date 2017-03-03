#include <Servo.h>
 
Servo esc;
int throttlePin = 130;
int inByte=-1;
void setup()
{
esc.attach(9);
Serial.begin(9600);
}
 
void loop()
{
  if (Serial.available()>0) {
    /* read the most recent byte */
    inByte = Serial.read();
  if (inByte == 'a')
    {     
     Serial.println("speed minus");
      throttlePin-=1;
    }
    else if(inByte == 'd')
    {
      throttlePin+=1;
    }
    }
int throttle = throttlePin;
throttle = map(throttle, 0, 1023, 0, 179);
Serial.print("input : ");
Serial.println(throttle);
Serial.print("Digit : ");
Serial.println(throttlePin);

esc.write(throttle);
}
