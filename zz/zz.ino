//#include <SomeSerial.h>

//#include <Wire.h>
//#include <SoftwareSerial_Class.h>
/* 
 
AUTHOR: Hazim Bitar (techbitar) 
DATE: Aug 29, 2013 
LICENSE: Public domain (use at your own risk) 
CONTACT: techbitar at gmail dot com (techbitar.com) 
EDIT FOR GALILEO: flummer (hackmeister.dk) 
 
    HC-05 GND --- Arduino GND Pin 
    HC-05 VCC (5V) --- Arduino 5V 
    HC-05 TX --- Arduino Pin 0 (hw RX) 
    HC-05 RX --- Arduino Pin1 (hw TX) 
    HC-05 Key (PIN 34) --- Arduino Pin 9 
*/  
#define Ch1 4
#define Ch2 5
#define Ch3 6
#define Ch4 7
#define Ch5 8
#define Ch6 9
#define Ch7 10
#define Ch8 11
//Define untuk Relay Module Active LOW
#define ON 0
#define OFF 1

//Define untuk Lampu LED atau Relay Module Active HIGH
//#define ON 1
//#define OFF 0

//Nama-nama panggilan untuk setiap channel Relay
//Silahkan diganti sesuai kebutuhan anda
//Misal : String Name1 = "Lampu Depan";
//INGAT! Kapitalisasi huruf Harus sesuai dengan RESULT Text yang tertera pada Aplikasi di Hp
String Name1 = "merah";
String Name2 = "kuning";
String Name3 = "hijau";
String Name4 = "biru";
String Name5 = "putih";
String Name6 = "magenta";
String Name7 = "ungu";
String Name8 = "jingga";
String All = "semua";

//3 Jenis Kata untuk Mendefinisikan kondisi ON
//Bisa dirubah sesuai kebutuhan misal String Key_ON_3="aktif";
String Key_ON_1 = "on";
String Key_ON_2 = "menyala";
String Key_ON_3 = "hidup";

//3 Jenis Kata untuk Mendefinisikan kondisi OFF
//Bisa dirubah sesuai kebutuhan misal String Key_OFF_2="nonaktif";
String Key_OFF_1 = "of";
String Key_OFF_2 = "padam";
String Key_OFF_3 = "mati";

//Notif ke Serial monitor atau ke LCD
String Notif_ON = " Dinyalakan";
String Notif_OFF = " Dimatikan";
void setup()  
{  
pinMode (Ch1,OUTPUT); digitalWrite (Ch1,OFF);
pinMode (Ch2,OUTPUT); digitalWrite (Ch2,OFF);
pinMode (Ch3,OUTPUT); digitalWrite (Ch3,OFF);
pinMode (Ch4,OUTPUT); digitalWrite (Ch4,OFF);
pinMode (Ch5,OUTPUT); digitalWrite (Ch5,OFF);
pinMode (Ch6,OUTPUT); digitalWrite (Ch6,OFF);
pinMode (Ch7,OUTPUT); digitalWrite (Ch7,OFF);
pinMode (Ch8,OUTPUT); digitalWrite (Ch8,OFF);
Serial.begin(9600);   
//Serial.begin(38400);  // HC-05 default speed in AT command more  
}  

  String Speech; 

void loop()  
{  
  
  // put your main code here, to run repeatedly:
 if(Serial.available()){
    char lastCharRead = Serial.read();
    Speech += lastCharRead; 
    //Serial.print (lastCharRead);
    //Serial.print (Speech);
       
       if (Speech.length() > 0) {
            Serial.print (Speech);
//************Membaca data Voice dan Melakukan index kata

            //Kendali Channel 1
            if(Speech.indexOf(Name1)>=0 && (Speech.indexOf(Key_ON_1)>=0 || Speech.indexOf(Key_ON_2)>=0 || Speech.indexOf(Key_ON_3)>=0)){
              //Action ON Channel 1
              Serial.print(Name1); Serial.println(Notif_ON);
              digitalWrite (Ch1,ON);
              Speech = "";} 
              else if(Speech.indexOf(Name1)>=0 && (Speech.indexOf(Key_OFF_1)>=0 || Speech.indexOf(Key_OFF_2)>=0 || Speech.indexOf(Key_OFF_3)>=0)){
              //Action OFF Channel 1
              Serial.print(Name1); Serial.println(Notif_OFF);
              digitalWrite (Ch1,OFF);
              Speech = "";}
           
            //Kendali Channel 2
            if(Speech.indexOf(Name2)>=0 && (Speech.indexOf(Key_ON_1)>=0 || Speech.indexOf(Key_ON_2)>=0 || Speech.indexOf(Key_ON_3)>=0)){
              //Action ON Channel 2
              Serial.print(Name2); Serial.println(Notif_ON);
              digitalWrite (Ch2,ON);
              Speech = "";} 
              else if(Speech.indexOf(Name2)>=0 && (Speech.indexOf(Key_OFF_1)>=0 || Speech.indexOf(Key_OFF_2)>=0 || Speech.indexOf(Key_OFF_3)>=0)){
              //Action OFF Channel 2
              Serial.print(Name2); Serial.println(Notif_OFF);
              digitalWrite (Ch2,OFF);
              Speech = "";}

              //Kendali Channel 3
            if(Speech.indexOf(Name3)>=0 && (Speech.indexOf(Key_ON_1)>=0 || Speech.indexOf(Key_ON_2)>=0 || Speech.indexOf(Key_ON_3)>=0)){
              //Action ON Channel 3
              Serial.print(Name3); Serial.println(Notif_ON);
              digitalWrite (Ch3,ON);
              Speech = "";} 
              else if(Speech.indexOf(Name3)>=0 && (Speech.indexOf(Key_OFF_1)>=0 || Speech.indexOf(Key_OFF_2)>=0 || Speech.indexOf(Key_OFF_3)>=0)){
              //Action OFF Channel 3
              Serial.print(Name3); Serial.println(Notif_OFF);
              digitalWrite (Ch3,OFF);
              Speech = "";}

              //Kendali Channel 4
            if(Speech.indexOf(Name4)>=0 && (Speech.indexOf(Key_ON_1)>=0 || Speech.indexOf(Key_ON_2)>=0 || Speech.indexOf(Key_ON_3)>=0)){
              //Action ON Channel 4
              Serial.print(Name4); Serial.println(Notif_ON);
              digitalWrite (Ch4,ON);
              Speech = "";} 
              else if(Speech.indexOf(Name4)>=0 && (Speech.indexOf(Key_OFF_1)>=0 || Speech.indexOf(Key_OFF_2)>=0 || Speech.indexOf(Key_OFF_3)>=0)){
              //Action OFF Channel 4
              Serial.print(Name4); Serial.println(Notif_OFF);
              digitalWrite (Ch4,OFF);
              Speech = "";}

              //Kendali Channel 5
            if(Speech.indexOf(Name5)>=0 && (Speech.indexOf(Key_ON_1)>=0 || Speech.indexOf(Key_ON_2)>=0 || Speech.indexOf(Key_ON_3)>=0)){
              //Action ON Channel 5
             Serial.print(Name5); Serial.println(Notif_ON);
              digitalWrite (Ch5,ON);
              Speech = "";} 
              else if(Speech.indexOf(Name5)>=0 && (Speech.indexOf(Key_OFF_1)>=0 || Speech.indexOf(Key_OFF_2)>=0 || Speech.indexOf(Key_OFF_3)>=0)){
              //Action OFF Channel 5
              Serial.print(Name5); Serial.println(Notif_OFF);
              digitalWrite (Ch5,OFF);
              Speech = "";}

              //Kendali Channel 6
            if(Speech.indexOf(Name6)>=0 && (Speech.indexOf(Key_ON_1)>=0 || Speech.indexOf(Key_ON_2)>=0 || Speech.indexOf(Key_ON_3)>=0)){
              //Action ON Channel 6
              Serial.print(Name6); Serial.println(Notif_ON);
              digitalWrite (Ch6,ON);
              Speech = "";} 
              else if(Speech.indexOf(Name6)>=0 && (Speech.indexOf(Key_OFF_1)>=0 || Speech.indexOf(Key_OFF_2)>=0 || Speech.indexOf(Key_OFF_3)>=0)){
              //Action OFF Channel 6
              Serial.print(Name6); Serial.println(Notif_OFF);
              digitalWrite (Ch6,OFF);
              Speech = "";}

              //Kendali Channel 7
            if(Speech.indexOf(Name7)>=0 && (Speech.indexOf(Key_ON_1)>=0 || Speech.indexOf(Key_ON_2)>=0 || Speech.indexOf(Key_ON_3)>=0)){
              //Action ON Channel 7
              Serial.print(Name7); Serial.println(Notif_ON);
              digitalWrite (Ch7,ON);
              Speech = "";} 
              else if(Speech.indexOf(Name7)>=0 && (Speech.indexOf(Key_OFF_1)>=0 || Speech.indexOf(Key_OFF_2)>=0 || Speech.indexOf(Key_OFF_3)>=0)){
              //Action OFF Channel 7
              Serial.print(Name7); Serial.println(Notif_OFF);
              digitalWrite (Ch7,OFF);
              Speech = "";}

              //Kendali Channel 8
            if(Speech.indexOf(Name8)>=0 && (Speech.indexOf(Key_ON_1)>=0 || Speech.indexOf(Key_ON_2)>=0 || Speech.indexOf(Key_ON_3)>=0)){
              //Action ON Channel 8
              Serial.print(Name8); Serial.println(Notif_ON);
              digitalWrite (Ch8,ON);
              Speech = "";} 
              else if(Speech.indexOf(Name8)>=0 && (Speech.indexOf(Key_OFF_1)>=0 || Speech.indexOf(Key_OFF_2)>=0 || Speech.indexOf(Key_OFF_3)>=0)){
              //Action OFF Channel 8
              Serial.print(Name8); Serial.println(Notif_OFF);
              digitalWrite (Ch8,OFF);
              Speech = "";}

              //Kendali Semua Channel
            if(Speech.indexOf(All)>=0 && (Speech.indexOf(Key_ON_1)>=0 || Speech.indexOf(Key_ON_2)>=0 || Speech.indexOf(Key_ON_3)>=0)){
              //Action ON All Channel
              Serial.print(All); Serial.println(Notif_ON);
              digitalWrite (Ch1,ON); digitalWrite (Ch2,ON); digitalWrite (Ch3,ON); digitalWrite (Ch4,ON);
              digitalWrite (Ch5,ON); digitalWrite (Ch6,ON); digitalWrite (Ch7,ON); digitalWrite (Ch8,ON);
              Speech = "";} 
              else if(Speech.indexOf(All)>=0 && (Speech.indexOf(Key_OFF_1)>=0 || Speech.indexOf(Key_OFF_2)>=0 || Speech.indexOf(Key_OFF_3)>=0)){
              //Action OFF All Channel
              Serial.print(All); Serial.println(Notif_OFF);
              digitalWrite (Ch1,OFF); digitalWrite (Ch2,OFF); digitalWrite (Ch3,OFF); digitalWrite (Ch4,OFF);
              digitalWrite (Ch5,OFF); digitalWrite (Ch6,OFF); digitalWrite (Ch7,OFF); digitalWrite (Ch8,OFF);
              Speech = "";}
             
               
      }   
   }    
}  

