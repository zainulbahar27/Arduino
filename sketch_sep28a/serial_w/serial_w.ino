#include <Servo.h>
Servo myservo;
Servo servo1;
int inByte=90;
int motor=90;
void setup() {                
// Turn the Serial Protocol ON
  myservo.attach(9);
  servo1.attach(10);
  Serial.begin(9600);
}

void loop() {
   /*  check if data has been sent from the computer: */
  if (Serial.available()>0) {
    /* read the most recent byte */
    inByte = Serial.read();
  if (inByte == 'a'&&motor>0)
    {     
     Serial.println("Gerak Kiri");
      motor-=1;
      myservo.write(motor);
      servo1.write(motor);
    }
   else if (inByte == 'q'&& motor>=1)
   {
    Serial.println("Gerak kiri terus");
      for(motor = motor; motor > 0; motor -= 1) 
      {                                  
        myservo.write(motor);
        servo1.write(motor);               
        delay(15);                        
      } 
   }
   else if (inByte == 'd')
    {
      Serial.println("Gerak kanan");
      motor+=1;
      myservo.write(motor);
      servo1.write(motor);
    }
    
   else if (inByte == 'e'&&motor<180)
   {
    Serial.println("Gerak kanan terus");
      for(motor=motor; motor < 180; motor += 1) 
      {                                
        myservo.write(motor);          
        servo1.write(motor);     
        delay(15);                        
      } 
   }
    }
    /*ECHO the value that was read, back to the serial port. */
}
