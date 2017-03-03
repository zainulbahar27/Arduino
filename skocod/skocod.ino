
#include <Servo.h> 
// membuat nama objek servo untuk pengontrolan servo 
Servo myservo;          
// variable untuk menyimpan posisi servo 
int pos = 90;      

void setup() {
  myservo.attach(9);
  myservo.write(pos);
  Serial.begin(57600);
}

void loop() {
  if(Serial.available()) {
    int roll = Serial.read();
    roll = Serial.parseInt();
    if(Serial.read() == '.') {
      Serial.println(roll);
    }

    if(roll > 8) {
      Serial.println("Putar Kanan");
      pos-=1;
      myservo.write(pos);
    } else if(roll < -8) {
      Serial.println("Putar Kiri");
      pos+=1;
      myservo.write(pos);
    }
    
  }
}

