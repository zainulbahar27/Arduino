int inByte=-1;
//pin tx bluetooth masuk ke pin rx galileo
//pin rx bluetooth masuk ke pin tx galileo
void setup()  
{  
  pinMode(8, OUTPUT);  // untuk mengaktifkan keylock  //sunnah
  Serial.begin(9600); // boudrate serial monitor
  Serial.println("Enter AT commands:");  
  Serial1.begin(9600);  //boudrate untuk komunikasi bluetooth
}  
  
void loop()  
{  
  
  if (Serial1.available())  {
    inByte=Serial1.read();
    Serial.write(inByte);
       if (inByte == '1'){ 
    digitalWrite(8,HIGH);
    Serial.println("LED nyala brooohh");
  }  
  else if(inByte=='0'){
    digitalWrite(8,LOW);
    Serial.println("LED mati brooohh");
  }
  }

  if (Serial.available()) { 
    Serial1.write(Serial.read());
    
   
  }
}
