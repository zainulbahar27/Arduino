#include <dht.h>
#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
dht DHT;
char Sched2[3], Leveltemp[2], jadwal2[3], TDMApacknumb2[2];
char RandNumber[5], LevelCheck[2];
char AlamatSendiri[5];
char AlamatParent[5];
char AlamatTerima[32];
char DataTerima[32];
char DataKirim[32];
char LevelKirim[2], TimeStamp1[4], TimeStamp2[4], TimeStamp4[4];
int  ubahmenit, ubahjam, detik, milidetik, menit, jam, ubahdetik, ubahmili, Level, tunggu, TS1, TS2, TS3, TS4, TimeStampInt;
char buffjam[3], buffmenit[3], buffdetik[3], buffmili[4];
unsigned long Latency, milibaru, detikbaru, menitbaru, jambaru;
int Sched, jadwal, jadwalpatent;
//unsigned long buffjam2, buffmenit2, buffdetik2, buffmili2;
boolean terdaftar, tersinkron;
int x,TDMApacknumb, detiklama, LevelClear;
char  milidetik2[4], detik2[3], menit2[3], jam2[3];

/* variabel data */
#define MQTTSN_TYPE_PUBLISH           0x0C
#define MQTTSN_TYPE_PUBACK            0x0D
#define MQTTSN_TOPIC_TYPE_PREDEFINED  0b01
#define MQTTSN_FLAG_QOS_1             0b01
#define MQTTSN_RC_ACCEPTED            0x00
#define MQTTSN_TOPIC_ID_SUHU          1
#define MQTTSN_TOPIC_ID_LEMBAB        2
#define MQTTSN_PROTOCOL_ID            0x01
#define MQTTSN_TYPE_CONNECT           0x04
#define MQTTSN_TYPE_CONNACK           0x05
#define DHT11_PIN 2 
byte datasensorack[4];
int con = 1;
int t = 0;


/*==========COMMON NODE===========*/

void setup(){
  Serial.begin(9600);
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init(); 
  Mirf.setRADDR((byte *)"node");  
  Mirf.payload = 32;
  Mirf.channel = 101;
  Mirf.config();
  Serial.println("Waiting...");
  terdaftar = 0;
  tersinkron = 0;
  tunggu = random(analogRead(A0),25000);
  while(tunggu%17000 < 2000 || tunggu < 14000){
    tunggu = random(analogRead(A0),25000);
  }
  x = 0;
  TDMApacknumb = 0;
  Sched = 0;
}

void loop(){
	if(Mirf.dataReady()){
           Mirf.getData((byte *)&DataTerima);
            LevelCheck[0] = DataTerima[8];
            LevelClear = strtoul(LevelCheck,NULL,0);
            if(!terdaftar && DataTerima[0] == 'B' && DataTerima[1] != 'B'){
              discovered();
              Serial.print("waktu tunggu ");
              Serial.println(tunggu);
              Serial.print("idle...");
            }else if(DataTerima[0] == 'S'){
              waktu();
              TS4 = milidetik;
              detiklama = detik;
              buffering();
              synchronized();
              waktu();
            }else if(DataTerima[0] == 'R' && LevelCheck != LevelKirim && tersinkron){
               //Serial.print("Levelcheck, Levelkirim: ");
               //Serial.print(LevelCheck);
               //Serial.println(LevelKirim);
               waktu();
               sprintf(TimeStamp2,"%d",milidetik);
               TimeStampInt = milidetik;
               answer_sync();
               waktu();
            }else if(DataTerima[0] == 'T' && LevelClear == (Level+1)){
               TDMArecv();
            }
            if(DataTerima[0] == 'X'){
               TDMAtempsend();
            }
      }else{
        if (!tersinkron && terdaftar && millis()%tunggu==1){
            synchronize();
            //tunggu = random(30000,30399);
            delay(1);
        }
        if(tersinkron){
            if(milidetik%999==0){
                //if(x==0 && detik > detikbaru ){ubahdetik=ubahdetik-1;}
                cetakwaktu();
                Serial.println("");
                delay(3);
                x++;
                if(x < 7){
                  discovery();
                }  
            }
            if(detik==jadwal && menit >= 1){
                TDMAsend();
                jadwal = jadwal + 2;
                if((jadwal-jadwalpatent)>3){jadwal = jadwalpatent;}
            }
         }
      }
   waktu();
}

void discovered(){
  //catat alamat parent
  for (int d=0; d<4; d++){
  AlamatParent[d] = DataTerima[d+1]; 
  }

  Serial.println("Paket Broadcast diterima!");
  Serial.print("Alamat Parent adalah: ");
  Serial.println(AlamatParent);
  
  //catat level
  Serial.println(DataTerima[5]);
  Leveltemp[0] = DataTerima[5];
  Level = strtoul(Leveltemp, NULL, 0);
  Level = Level+1;
  Serial.print("Node bergabung di hierarki level: ");
  Serial.println(Level);
  
  //ubah alamat sendiri
  UbahAlamat();
  Serial.print("Node berubah alamat menjadi: ");
  Serial.println(AlamatSendiri);
  Mirf.setRADDR((byte *)AlamatSendiri);
  
  terdaftar = 1;
}

void UbahAlamat(){
  int Acak = random(analogRead(A0),9999); //generate random number
  while(Acak<1000){
  Acak = random(analogRead(A0),9999);
  }
  sprintf(AlamatSendiri,"%d", Acak);
  }

void synchronize(){
  waktu();
  Mirf.setTADDR((byte *)AlamatParent);
  strcpy(DataKirim,"R");
  strcat(DataKirim,AlamatSendiri);
  sprintf(TimeStamp1,"%d",milidetik);
  if(milidetik<10){
  strcat(DataKirim,"0");
  strcat(DataKirim,"0");
  }else if(milidetik<100){ 
  strcat(DataKirim,"0");
  }
  strcat(DataKirim,TimeStamp1);
  sprintf(LevelKirim,"%d", Level);
  strcat(DataKirim,LevelKirim);
  //strcat(DataKirim,Latency);
  Latency = millis();
  Mirf.send((byte *)&DataKirim);
  while(Mirf.isSending()){
		Serial.println("Request Sinkronisasi terkirim!");
                Serial.println(DataKirim);
                delay(2);
		}
}

void synchronized(){
   Serial.print("Waktu awal: ");
   waktu();
   cetakwaktu();
   Serial.println("");
   waktu();
   Latency = millis() - Latency;
   ubahwaktu();
   waktu();
   cetakwaktu();
   Serial.println("(Tersinkronisasi!))");
   Serial.print("Dari paket: ");
   Serial.println(DataTerima);
   Serial.print("Delay total: ");
   Serial.print(Latency);
   Serial.print(", delay propagasi: ");
   Serial.println(TS4);
   Serial.print("Jadwal: ");
   Serial.println(jadwalpatent);
   if (TS4<20 && TS4>0){
   tersinkron = 1;
   }
   Sched = jadwalpatent + 30;
   //Serial.println(ubahmili);
   //Serial.println(ubahdetik);
}

void waktu(){    
  milidetik = millis()%1000;
  detik = (millis() + ubahmili)/1000;
  milidetik = milidetik + ubahmili;
  detik = detik + ubahdetik;
  menit = detik/60;
  menit = menit + ubahmenit;
  jam=menit/60;
  jam = jam + ubahjam;
  
  if(milidetik>999){//reset milidetik
  milidetik = milidetik%1000; 
  }else if(milidetik<0){
  milidetik = 1000+milidetik; 
  }
  
  if(detik>59){//reset detik
  detik=detik%60;
  }else if(detik<0){
  detik = detik + 60;
  }
  
  if(menit>59){//reset menit
  menit=menit%60;
  }else if(menit<0){
  menit = menit + 60;
     
  if(jam>23){//reset jam
  jam=jam%24;
  }
}
}
 
void cetakwaktu(){
  Serial.print(jam);Serial.print(":");
  Serial.print(menit);Serial.print(":");
  Serial.print(detik);Serial.print(":");    
  Serial.print(milidetik);
  //delay(1000);
}
 
void ubahwaktu(){
  ubahmili = milibaru - milidetik;
  ubahdetik = detikbaru - detiklama;
  ubahmenit = menitbaru - menit;
  ubahjam = jambaru - jam;
  ubahmili = ubahmili + TS4;
}

void discovery(){
        sprintf(LevelKirim,"%d", Level);
        Mirf.setTADDR((byte *)"node");
        strcpy(DataKirim,"B"); //first, adding header character
        strcat(DataKirim,AlamatSendiri); //concat 2 var
        strcat(DataKirim,LevelKirim);
        Mirf.send((byte *)&DataKirim);
        Serial.println("Paket discovery terkirim!");
        while(Mirf.isSending()){
		
                //Serial.println(AlamatSendiri);
                //Serial.println(DataKirim);
                //delay(2);
		}
}

void kirimwaktu(){
  waktu();
  sprintf(milidetik2,"%d", milidetik);
  sprintf(detik2,"%d", detik);
  sprintf(menit2,"%d", menit);
  sprintf(jam2,"%d", jam);
  
  strcpy(DataKirim,"S"); //first, adding header character
  if(jam < 10){strcat(DataKirim,"0");}
  strcat(DataKirim,jam2);
  strcat(DataKirim,":");
  if(menit < 10){strcat(DataKirim,"0");}
  strcat(DataKirim,menit2);
  strcat(DataKirim,":");
  if(detik < 10){strcat(DataKirim,"0");}
  strcat(DataKirim,detik2);
  strcat(DataKirim,":");
  if(milidetik < 10){
  strcat(DataKirim,"0");
  strcat(DataKirim,"0");
  }else if(milidetik < 100){
  strcat(DataKirim,"0");
  }
  strcat(DataKirim,milidetik2);
}

void answer_sync(){
  TDMAsched();
  for (int e=0; e<4; e++){
  AlamatTerima[e] = DataTerima[e+1]; 
  }
  for (int f=0; f<3; f++){
  TimeStamp1[f] = DataTerima[f+5]; 
  }
  Serial.print("Ada permintaan sinkronisasi dari node ");
  Serial.println(AlamatTerima);
  
  Mirf.setTADDR((byte *)AlamatTerima);//kirim balasan
  kirimwaktu();
  strcat(DataKirim,TimeStamp1);//timestamp1
  if(TimeStampInt<10){
  strcat(DataKirim,"0");
  strcat(DataKirim,"0");
  }else if(TimeStampInt<100){
  strcat(DataKirim,"0");
  }
  strcat(DataKirim,TimeStamp2);//timestamp2
  sprintf(Sched2,"%d", Sched);
  strcat(DataKirim,Sched2);
  Serial.println(Sched2);
  Mirf.send((byte *)&DataKirim);
  while(Mirf.isSending()){
      Serial.print("Paket synchronization terkirim pada ");
      cetakwaktu();
  }
}

void buffering(){
               buffjam[0] = DataTerima[1];
               buffjam[1] = DataTerima[2];
               buffmenit[0] = DataTerima[4];
               buffmenit[1] = DataTerima[5];
               buffdetik[0] = DataTerima[7];
               buffdetik[1] = DataTerima[8];
               buffmili[0] = DataTerima[10];
               buffmili[1] = DataTerima[11];
               buffmili[2] = DataTerima[12]; 
               TimeStamp2[0] = DataTerima[16];
               TimeStamp2[1] = DataTerima[17];
               TimeStamp2[2] = DataTerima[18];
               milibaru = strtoul(buffmili,NULL,0);
               detikbaru = strtoul(buffdetik,NULL,0);
               menitbaru = strtoul(buffmenit,NULL,10);
               jambaru = strtoul(buffjam,NULL,10);
               TS1 = strtoul(TimeStamp1,NULL,0);
               TS2 = strtoul(TimeStamp2,NULL,0);
               TS3 = milibaru;
               TS4 = ((TS2 - TS1) + (TS4 - TS3))/2;
               jadwal2[0] =  DataTerima[19];
               jadwal2[1] =  DataTerima[20];
               jadwal = strtoul(jadwal2,NULL,10);
               jadwalpatent = jadwal;
}

void TDMAsend(){
  TDMApacknumb = TDMApacknumb + 1;
  TDMApacknumb = TDMApacknumb%10; 
  sprintf(TDMApacknumb2,"%d", TDMApacknumb);
  Mirf.setTADDR((byte *)"1111");
  strcpy(DataKirim,"T");
  strcat(DataKirim,AlamatSendiri);
  strcat(DataKirim,TDMApacknumb2);
  strcat(DataKirim,"0");
  sprintf(LevelKirim,"%d", Level);
  strcat(DataKirim,LevelKirim);
  Mirf.send((byte *)&DataKirim);
  while(Mirf.isSending());
  Serial.println("Mengirim sensor");
  //Serial.println(AlamatParent);
}

void TDMAtempsend(){
  Mirf.setTADDR((byte *)"1111");
  //int temp = DHT.temperature;
  byte datasensorsend[6];
  DHT.read11(DHT11_PIN);
  float suhu = DHT.temperature;
  // Read humidity
  //float temp = DHT.computeHeatIndex(suhu, false);
  byte msg_typ = MQTTSN_TYPE_PUBLISH;
  byte qos = MQTTSN_FLAG_QOS_1;
  byte topic_typ = MQTTSN_TOPIC_TYPE_PREDEFINED;
  byte msg_id = t;
  int topic_id = MQTTSN_TOPIC_ID_SUHU;
  float data = suhu;  
  datasensorsend[0] = qos;
  datasensorsend[1] = topic_id;
  datasensorsend[2] = msg_id; 
  datasensorsend[3] = msg_typ; 
  datasensorsend[4] = data;
  datasensorsend[5] = topic_typ;
  Mirf.setTADDR((byte *)"1111");
  Mirf.send((byte *)&datasensorsend);
  while(Mirf.isSending());
  Serial.println("data temperature sent!");  
  //Serial.println(AlamatParent);  
}

void TDMArecv(){
  for (int c=0; c<4; c++){
  AlamatTerima[c] = DataTerima[c+1]; 
  }
  Serial.print("Data TDMA diterima dari ");
  Serial.print(AlamatTerima);
  Serial.print("(");
  Serial.print(DataTerima[5]);
  Serial.print(")");
  Serial.println();
}

void TDMAsched(){ 
  Sched = Sched + 10;
}

