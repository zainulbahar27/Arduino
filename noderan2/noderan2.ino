/* library */
#include <DHT.h>
#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* variabel setting waktu */
int  ubahmenit, ubahjam, detik, milidetik, menit, jam, ubahdetik, ubahmili, Level, tunggu, TS1, TS2, TS3, TS4, TimeStampInt, detiklama;
char buffjam[3], buffmenit[3], buffdetik[3], buffmili[4];
unsigned long Latency, milibaru, detikbaru, menitbaru, jambaru;
char  milidetik2[4], detik2[3], menit2[3], jam2[3];

/* variabel tpsn dan tdma */
char Sched2[3], Leveltemp[2], jadwal2[3], TDMApacknumb2[2];
char RandNumber[5], LevelCheck[2];
char AlamatSendiri[5];
char AlamatParent[5];
char AlamatTerima[32];
char datarecv[32];
char datasend[32];
int Sched, jadwal, jadwalpatent;
boolean terdaftar, tersinkron;
int x,TDMApacknumb, LevelClear;
char LevelKirim[2], TimeStamp1[4], TimeStamp2[4], TimeStamp4[4];

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
#define DHTPIN 2 
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

byte datasensorsend[7];
byte datasensorack[5];
int con = 1;
int t = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init(); 
  Mirf.setRADDR((byte *)"node");  
  Mirf.payload = 32;
  Mirf.channel = 101;
  Mirf.config();
  Serial.println("Please Wait...");
  byte msg_typ = MQTTSN_TYPE_CONNECT;
  byte protocol_id = MQTTSN_PROTOCOL_ID;
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
void loop() {
  // put your main code here, to run repeatedly:
  if(Mirf.dataReady()){        
    Mirf.getData((byte*)&datarecv);
    LevelCheck[0] = datarecv[8];
    LevelClear = strtoul(LevelCheck,NULL,0);
    if(!terdaftar && datarecv[0] == 'D'){
      discovered();
      } 
    else if(datarecv[0] == 'W'){
      TS4 = milidetik;
      detiklama = detik;
      buffering();
      synchronized();
      }   
  }
  else{
    if(terdaftar && !tersinkron && millis()%tunggu==1){
      synchronize();
      delay(1);
      }
    else if(tersinkron){
      if(milidetik%999 == 0){
        cetakwaktu();
	      delay(3);       
        }
        TDMAtempsend();      
      }    
    }
    waktu();
}
  /*
              waktu();
              TS4 = milidetik;
              detiklama = detik;
              waktu();
              cetakwaktu();
  */

void ubahwaktu(){
  ubahmili = milibaru - milidetik;
  ubahdetik = detikbaru - detiklama;
  ubahmenit = menitbaru - menit;
  ubahjam = jambaru - jam;
  ubahmili = ubahmili + TS4;
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
  Serial.println("");
  //delay(10);
}

void discovered(){
  //catat alamat parent
  for (int d=0; d<4; d++){
  AlamatParent[d] = datarecv[d+1]; 
  }
  Serial.println("data from root already received!");
  Serial.print("root address: ");
  Serial.println(AlamatParent);  
  //catat level
  Serial.print("your level node now is: ");
  Serial.println(datarecv[5]);
  Leveltemp[0] = datarecv[5];
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
  strcpy(datasend,"S");
  strcat(datasend,AlamatSendiri);
  sprintf(TimeStamp1,"%d",milidetik);
  if(milidetik<10){
    strcat(datasend,"0");
    //strcat(datasend,"0");
    }
  else if(milidetik<100){ 
    strcat(datasend,"0");
    }
  strcat(datasend,TimeStamp1);
  sprintf(LevelKirim,"%d", Level);
  strcat(datasend,LevelKirim);
  //strcat(DataKirim,Latency);
  Latency = millis();
  Mirf.send((byte *)&datasend);
  while(Mirf.isSending()){
    Serial.println("Request Sinkronisasi sent!");
    //Serial.println(datasend);
    delay(2);
    }
}

void buffering(){
   buffjam[0] = datarecv[1];
   buffjam[1] = datarecv[2];
   buffmenit[0] = datarecv[4];
   buffmenit[1] = datarecv[5];
   buffdetik[0] = datarecv[7];
   buffdetik[1] = datarecv[8];
   buffmili[0] = datarecv[10];
   buffmili[1] = datarecv[11];
   buffmili[2] = datarecv[12]; 
   TimeStamp2[0] = datarecv[16];
   TimeStamp2[1] = datarecv[17];
   TimeStamp2[2] = datarecv[18];
   milibaru = strtoul(buffmili,NULL,0);
   detikbaru = strtoul(buffdetik,NULL,0);
   menitbaru = strtoul(buffmenit,NULL,10);
   jambaru = strtoul(buffjam,NULL,10);
   TS1 = strtoul(TimeStamp1,NULL,0);
   TS2 = strtoul(TimeStamp2,NULL,0);
   TS3 = milibaru;
   TS4 = ((TS2 - TS1) + (TS4 - TS3))/2;
   jadwal2[0] =  datarecv[19];
   jadwal2[1] =  datarecv[20];
   jadwal = strtoul(jadwal2,NULL,10);
   jadwalpatent = jadwal;
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
   Serial.println("(synchronized!))");
   Serial.print("from packet: ");
   Serial.println(datarecv);
   Serial.print("delay total: ");
   Serial.println(Latency);
   Serial.print("propagasi: ");
   Serial.println(TS4);
   //Serial.print("Scheduled: ");
   //Serial.println(jadwalpatent);
   if (TS4<20 && TS4>0){
     tersinkron = 1;
     }    
   //Sched = jadwalpatent + 30;
   //Serial.println(ubahmili);
}

void TDMAtempsend(){
  TDMApacknumb = TDMApacknumb + 1;
  TDMApacknumb = TDMApacknumb%10; 
  sprintf(TDMApacknumb2,"%d", TDMApacknumb);
  t+=1;
  //int temp = DHT.temperature;
  float suhu = dht.readTemperature();
  // Read humidity
  float temp = dht.computeHeatIndex(suhu, false);
  byte msg_typ = MQTTSN_TYPE_PUBLISH;
  byte qos = MQTTSN_FLAG_QOS_1;
  byte topic_typ = MQTTSN_TOPIC_TYPE_PREDEFINED;
  byte msg_id = t;
  int topic_id = MQTTSN_TOPIC_ID_SUHU;
  float data = temp;  
  datasensorsend[0] = qos;
  datasensorsend[1] = topic_id;
  datasensorsend[2] = msg_id; 
  datasensorsend[3] = msg_typ; 
  datasensorsend[4] = data;
  datasensorsend[5] = topic_typ;
  Mirf.setTADDR((byte *)"1111");
  Mirf.send((byte *)&datasensorsend);
  Serial.println("data temperature sent!");
  Serial.println(data);  
  delay(500);
  //Serial.println(AlamatParent);
}
