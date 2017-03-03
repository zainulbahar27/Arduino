#include <Servo.h>

Servo myservo;

int servoPin = 7;
long duration;
int distance;
long duration2;
int distance2;
#define echoPin 8
#define triggerPin 9

#define echoPin2 10
#define triggerPin2 11

void setup()
{
  Serial.begin(9600);
  pinMode(echoPin, INPUT);     //echoPin dijadikan input
  pinMode(triggerPin, OUTPUT); //triggerpin jadi output
  pinMode(echoPin2, INPUT);     //echoPin dijadikan input
  pinMode(triggerPin2, OUTPUT); //triggerpin jadi output
  pinMode(4, OUTPUT);
  myservo.attach(servoPin);
  myservo.write(177);
long duration;
int distance;
long duration2;
int distance2;
digitalWrite(4,HIGH ); 
}


void loop()
{
  if (Serial.available())
  {
     delay(200);
     char ch = Serial.read();
     if (ch == 'c')
     {
      while(1)
      {
          digitalWrite(triggerPin, LOW);//menghapus triggerPin
          delayMicroseconds(2); 
          digitalWrite(triggerPin, HIGH); //membuat trigger 10mikro second/usec
          delayMicroseconds(10);
          digitalWrite(triggerPin, LOW); 
          duration = pulseIn(echoPin, HIGH); //membaca pulse HIGH yang dikirim sensor dalam bentuk waktu usec
          distance = duration*(0.034/2); //jarak yang dihasilkan dalam bentuk cm, rumus : pulse(us)/58
          delayMicroseconds(2); 
          digitalWrite(triggerPin, LOW);//menghapus triggerPin
          //Serial.println("ini loop baca sensor");
          if (distance < 5)
          {
            Serial.print("y\n");
            break;
          }
            delay(200);
      }
     }
     else if ( ch == 'o' )
     {
      myservo.write(95);
      Serial.print("b\n");
      
      while(1)
      {
          delay(200);
          bool flag = false;
          digitalWrite(triggerPin2, LOW);//menghapus triggerPin
          delayMicroseconds(2); 
          digitalWrite(triggerPin2, HIGH); //membuat trigger 10mikro second/usec
          delayMicroseconds(10);
          digitalWrite(triggerPin2, LOW); 
          duration2 = pulseIn(echoPin2, HIGH); //membaca pulse HIGH yang dikirim sensor dalam bentuk waktu usec
          distance2 = duration2*(0.034/2); //jarak yang dihasilkan dalam bentuk cm, rumus : pulse(us)/58
          delayMicroseconds(2); 
          digitalWrite(triggerPin2, LOW);//menghapus triggerPin
          //Serial.println("ini loop baca kendaraan");
            if( distance2 < 10  )
            {
               //Serial.println("delay 3 detik");
               delay(1000);
                 while(1)
              {
                  digitalWrite(triggerPin2, LOW);//menghapus triggerPin
                  delayMicroseconds(2); 
                  digitalWrite(triggerPin2, HIGH); //membuat trigger 10mikro second/usec
                  delayMicroseconds(10);
                  digitalWrite(triggerPin2, LOW); 
                  duration2 = pulseIn(echoPin2, HIGH); //membaca pulse HIGH yang dikirim sensor dalam bentuk waktu usec
                  distance2 = duration2*(0.034/2); //jarak yang dihasilkan dalam bentuk cm, rumus : pulse(us)/58
                  delayMicroseconds(2); 
                  digitalWrite(triggerPin2, LOW);//menghapus triggerPin
                  // Serial.println("ini loop kendaraan melewati");
                if( distance2 > 15)
                {
                  flag = true;
                  break;
                }
                delay(200);
                }
            }
            if (flag == true)
            {
              myservo.write(170);
              Serial.print("t\n");
              flag = false;
              break;
            }
      } 
     }
  }
  }
