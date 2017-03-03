int inByte=-1;
int probe =5;
int mostureSensor = A0;
void setup()  
{ 
  pinMode(probe, OUTPUT); 
  pinMode(8, OUTPUT);  // this pin will pull the HC-05 pin 34 (key pin) HIGH to switch module to AT mode  
  Serial.begin(9600);  
  Serial.println("Enter AT commands:");  
  Serial1.begin(9600);  // HC-05 default speed in AT command more  
  Serial1.println("Bacin Tanaman");
  Serial1.println("===================");
  Serial1.println("0 : Cek Status Air");
  Serial1.println("1 : Cek Status cahaya");
  Serial1.println("2 : Auto Siram");
  Serial1.println("3 : Manual Siram");
  Serial1.println("===================");
  Serial1.println("Menunggu Intruksi");
  Serial1.println("===================");
}  

void loop()  
{  
  digitalWrite(probe, HIGH);
  delay(1000);
  int kelembaban = analogRead(mostureSensor);
  // Keep reading from HC-05 and send to Arduino Serial Monitor  
  if (Serial1.available())  {
    inByte=Serial1.read();
    Serial.write(inByte);
    if (inByte == '0')
    { 
      if (kelembaban >= 820)

      {
        Serial1.println("Becek boss, -_- gak enak badan"); 

      }

      else if (kelembaban >= 615 && kelembaban < 820)

      {

        Serial1.println("seger boss");
      }

      else if (kelembaban >= 410 && kelembaban < 615)

      {
        Serial1.println("Lumayan");
      }

      else if (kelembaban >= 250 && kelembaban < 410)

      {
        Serial1.println("Tenggorokan Kering");
      }

      else if (kelembaban >= 0 && kelembaban < 250)

      {

        Serial1.println("Haus Pool sam -_- ");

      }

    }  
    else if(inByte=='1')
    {
    if (kelembaban >= 820)

      {
        Serial1.println("kalau jam 15.00 WIB kebawah, hot banget sam"); 

      }

      else if (kelembaban >= 615 && kelembaban < 820)

      {

        Serial1.println("kalau jam 15.00 WIB kebawah, Lumayan lah");
      }

      else if (kelembaban >= 410 && kelembaban < 615)

      {

        Serial1.println("kalau jam 15.00 WIB kebawah, Please asumsi cahaya matahari");

        
    }
    }
    else if(inByte=='2')
    {
      digitalWrite(9,HIGH);
      delay(5000);
      digitalWrite(9,LOW);
    }
    delay(500);
  }
  // Keep reading from Arduino Serial Monitor and send to HC-05  
  if (Serial.available()) { 
    Serial1.write(Serial.read());


  }
}

