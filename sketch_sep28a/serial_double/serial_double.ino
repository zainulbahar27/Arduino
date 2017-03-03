#include <Servo.h>
Servo myservo;
Servo servo1;
Servo tengah;
//Servo serbawah;
int inByte=-1;
int motor9=99;
int motor10=86;
int motort=90;
void setup() {                
// Turn the Serial Protocol ON
  myservo.attach(9);
  servo1.attach(10);
  tengah.attach(11);
  Serial.begin(9600);
}

void loop() {
      myservo.write(motor9);
      servo1.write(motor10);
    tengah.write(motort);
   /*  check if data has been sent from the computer: */
  if (Serial.available()>0) {
    /* read the most recent byte */
    inByte = Serial.read();
  if (inByte == 'a'&&motor9>0&&motor10<180)
    {     
     Serial.println("Gerak Kiri");
      motor9-=1;
      motor10+=1;
      myservo.write(motor9);
      servo1.write(motor10);
    }
   else if (inByte == 'q'&& motor9>=1&&motor10<=179)
   {
    Serial.println("Gerak kiri terus");
      while(motor9>0&&motor10<180) 
      {                                  
      motor9-=1;
      motor10+=1;
      myservo.write(motor9);
      servo1.write(motor10);               
        delay(15);                     
      } 
   }
   else if (inByte == 'd'&& motor9<=179&&motor10>=1)
    {
      Serial.println("Gerak kanan");
      motor9+=1;
      motor10-=1;
      myservo.write(motor9);
      servo1.write(motor10);
    }
    
   else if (inByte == 'e'&& motor9<=179&&motor10>=1)
   {
    Serial.println("Gerak kanan terus");
      while(motor9<=179&&motor10>=1) 
      {                                
      motor9+=1;
      motor10-=1;
      myservo.write(motor9);
      servo1.write(motor10);    
      delay(15);                        
      }
    }

     if (inByte == 'x'&&motort>0)
    {     
     Serial.println("Gerak Kiri");
      motort-=1;
      tengah.write(motort);  
    }
   else if (inByte == 's'&& motort>=1)
   {
    Serial.println("Gerak kiri terus");
      while(motort>0) 
      {                      
      motort-=1;
      tengah.write(motort);                 
        delay(15);                     
      } 
   }
   else if (inByte == 'z'&& motort<=179)
    {
      Serial.println("Gerak kanan");
      motort+=1;
      tengah.write(motort);  
    }
    
   else if (inByte == 'w'&& motort<=179)
   {
    Serial.println("Gerak kanan terus");
      while(motort<=179) 
      {                                
      motort+=1;
      tengah.write(motort);    
      delay(15);                        
      }
    }
   /*ECHO the value that was read, back to the serial port. */
}
}
