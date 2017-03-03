int inByte=-1;
int mostureSensor = A0;
void setup()  
{ 
  pinMode(8, OUTPUT);  
  Serial.begin(9600);
  Serial.println("Enter AT commands:");  
  Serial1.begin(9600);  
  Serial1.println("Bacin Tanaman");
  Serial1.println("===================");
  Serial1.println("0 : Cek Status Air");
  Serial1.println("1 : Siram Air");
  Serial1.println("2 : Matikan Air");
  Serial1.println("===================");
  Serial1.println("Menunggu Intruksi");
  Serial1.println("===================");
  Serial1.println(" ");
}  

void loop()  
{  
  int kelembaban = analogRead(A1);
  Serial.println(kelembaban);
  delay(1000);
  // Keep reading from HC-05 and send to Arduino Serial Monitor  
  if (Serial1.available())  {
    inByte=Serial1.read();
    Serial.write(inByte);
    if (inByte == '0')
    { 
      if (kelembaban >= 820)
      {
        Serial1.println("Haus Pool sam -_-"); 
      }

      else if (kelembaban >= 615 && kelembaban < 820)
      {
        Serial1.println("Tenggorokan Kering");
      }

      else if (kelembaban >= 410 && kelembaban < 615)
      {
        Serial1.println("Lumayan");
      }
      else if (kelembaban >= 250 && kelembaban < 410)
      {
        Serial1.println("seger boss");
      }
      else if (kelembaban >= 0 && kelembaban < 250)
      {
        Serial1.println("Becek boss, -_- gak enak badan ");
      }
    }  
    
    else if(inByte=='1')
    {
      digitalWrite(8,HIGH);
      Serial1.println("air Menyala");
    }
    
    else if(inByte=='2')
    {
      digitalWrite(8,LOW);
      Serial1.println("air Mati");
    }
    
    delay(500);
  }
  // Keep reading from Arduino Serial Monitor and send to HC-05  
  if (Serial.available()) { 
    Serial1.write(Serial.read());


  }
}

