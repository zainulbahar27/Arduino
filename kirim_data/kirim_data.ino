#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 11);
int led = 13;
int flag=0;
double masukan[8][8] =        {{468,525,417,417,524,87.41,76.69,-10.48},{519,555,419,420,575,94.77,81.81,-4.06},
{527,572,465,444,593,87.75,81.52,4.46},{473,548,459,443,548,81.47,81.67,-8.32},{476,519,419,418,515,95.54,77.31,-4.42},{510,522,445,445,536,106.2,70.73,-2.74},
{509,553,446,436,550,104.71,71.36,-0.13},{490,519,418,420,551,87.03,83.72,-5.77}};
void setup() {
  // initialise serial communications at 9600 bps:
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(led, OUTPUT);
  digitalWrite(led, HIGH);
}
 
void loop() {
  if(mySerial.available()){
  char data = mySerial.read();
 if(data=='1')
  {
      sendAndroidValues();
      Serial.println("Data Berhasil Di Kirim");
  }
  else if(data=='2')
  {
    flag++;
          Serial.println("Data Berhasil di proses");
  }
  else if(data=='3')
  {
          sendAndroidValues();
      Serial.println("Data Di Kirim ulang");
  }
   delay(100);  
}
}
//sends the values from the sensor over serial to BT module
void sendAndroidValues()
 {
      if(flag>7)
   {
     flag=0;
   }
    delay(500);        //added a delay to eliminate missed transmissions
  //puts # before the values so our app knows what to do with the data
  mySerial.print('#');
   Serial.print('#');
  //for loop cycles through 4 sensors and sends values via serial
  for(int k=0; k<8; k++)
  {
    mySerial.print(masukan[flag][k]);
    mySerial.print('+');
    Serial.print(masukan[flag][k]);
    Serial.print('+');
    //technically not needed but I prefer to break up data values
    //so they are easier to see when debugging
  }
 mySerial.print('~'); //used as an end of transmission character - used in app for string length
 mySerial.println();
  Serial.print('~'); //used as an end of transmission character - used in app for string length
 Serial.println();
 delay(100);        //added a delay to eliminate missed transmissions
}
