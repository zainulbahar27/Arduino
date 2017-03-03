/* Arduino Servo Test sketch */
#include <Servo.h>
Servo servoMain; // Define our Servo
void setup()
{   
servoMain.attach(8); // servo on digital pin 10 
}
void loop()
{
  servoMain.write(10); 
  delay(1000);
  servoMain.write(20);
 delay(1000); 
   servoMain.write(30); 
  delay(1000);
  servoMain.write(40); 
  delay(1000);
   servoMain.write(50); 
  delay(1000);
  servoMain.write(60); 
  delay(1000);
  servoMain.write(70);
 delay(1000); 
   servoMain.write(80); 
  delay(1000);
  servoMain.write(90); 
  delay(1000);
   servoMain.write(100); 
  delay(1000);
  servoMain.write(110);
 
   servoMain.write(120); 
  delay(1000);
  servoMain.write(130); 
}
