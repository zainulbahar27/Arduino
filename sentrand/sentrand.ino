#include <DistanceGP2Y0A21YK.h>
#include <Servo.h>

DistanceGP2Y0A21YK Dist;
int distance;
Servo myservo;
int pos = 0; 
int flag = 0;
int photodioda = 1;
        
void setup()
{
  Serial.begin(9600);
  pinMode(7, OUTPUT); //LED
  digitalWrite(7, HIGH);
  pinMode(2, OUTPUT); //PhotoDioda
  digitalWrite(2, HIGH);
  Dist.begin(A0);
  myservo.attach(9);
}
 
void loop()
{
  distance = Dist.getDistanceCentimeter();
 // int valsharp = analogRead(sensorsharp); 
  int valphoto = analogRead(photodioda); 
  Serial.print("Value Photodioda = ");
  Serial.println(valphoto, DEC);
  Serial.print("Value Sharp = ");     
  Serial.println(distance);
  
  if(valphoto < 500) { //ada piring
    if(distance > 10) {//piring kosong
      if(flag==0||flag==2){
      for (pos = 90; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
      myservo.write(pos);
      Serial.println("buang piring");// tell servo to go to position in variable 'pos'
      delay(5);                       // waits 15ms for the servo to reach the position
   }
   flag=1;
    }
    }else {
      if(flag==0||flag==1)
      {
       for (pos = 0; pos <= 90; pos += 1){
         myservo.write(pos);
         Serial.println("biarkan piring");
        delay(5); 
      }
      flag=2;
      }
    }
  
  delay(500);                    
  }
}
