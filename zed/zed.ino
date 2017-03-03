int inByte=-1;
int mostureSensor = A0;
int ldr = A1 ;
int xldr=0;
int xmosture=0;
void setup()  
{  
  Serial.begin(9600);
  Serial.println("Enter AT commands:");  
  Serial1.begin(9600);  
  Serial1.println("Bacin Tanaman");
  Serial1.println("===================");
  Serial1.println("0 : Cek Status Air");
  Serial1.println("1 : Cek Status Cahaya");
  
  Serial1.println("===================");
  Serial1.println("=Fotosintesis Pukul 07.00 - 15.00 WIB=");
  Serial1.println("Menunggu Intruksi");
  Serial1.println("===================");
  Serial1.println(" ");
}  
void loop()  
{  
  int kelembaban = analogRead(mostureSensor);
  delay(200);
  int cahaya = analogRead(ldr);
  delay(200);
  Serial.print("Nilai kelembaban = ");
  Serial.println(kelembaban);
  Serial.print("Nilai intensitas = ");
  Serial.println(cahaya);
  delay(1000);
  // Keep reading from HC-05 and send to Arduino Serial Monitor  
  if (Serial1.available())  {
    inByte=Serial1.read();
    Serial.print("input user = ");
    Serial.println(inByte);
    if (inByte == '0')
    { 
      if (kelembaban >= 410)
      {
        Serial1.println("Tanaman kekeringan sangat membutuhkan air"); 
      }

      else if (kelembaban >= 307 && kelembaban < 410)
      {
        Serial1.println("Tanaman membutuhkan air");
      }

      else if (kelembaban >= 205 && kelembaban < 307)
      {
        Serial1.println("Air Sudah sesuai kebutuhan");
      }
      else if (kelembaban >= 125 && kelembaban < 205)
      {
        Serial1.println("Air sudah cukup");
      }
      else if (kelembaban >= 0 && kelembaban < 125)
      {
        Serial1.println("Air terlalu banyak");
      }
    }  

    else if(inByte=='1')
    {
      if (cahaya<450)
      {
        Serial1.println("Tanaman berfotosintesis"); 
      }

      else
      {
        Serial1.println("Tanaman membutuhkan cahaya MH untuk berfotosintesis");
      }
    }
    delay(500);
  }

  if (kelembaban>375&&xmosture==0)
  {
    xmosture=1;
    Serial1.println("urgent Tanaman membutuhkan air");
  }
  else if(kelembaban<=375&&xmosture==1)
  {
    xmosture=0;
  }

  if (cahaya>475&&xldr==0)
  {
    xldr=1;
    Serial1.println("urgent tidak ada cahaya untuk berfotosintesis");
    Serial1.println("=======Untuk Pukul 07.00 - 15.00 WIB=========");
  }
  else if(cahaya<=475&&xldr==1)
  {
    xldr=0;
  }
  Serial.println(xmosture);
  Serial.println(xldr);
}

