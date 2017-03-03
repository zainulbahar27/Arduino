/* library */
#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* variabel setting waktu */
int  ubahmenit, ubahjam, detik, milidetik, menit, jam, ubahdetik, ubahmili, tunggu, TS1, TS2, TS3, TS4, TimeStampInt, detiklama;
char buffjam[3], buffmenit[3], buffdetik[3], buffmili[4];
unsigned long Latency, milibaru, detikbaru, menitbaru, jambaru;
char  milidetik2[4], detik2[3], menit2[3], jam2[3];

/* variabel tpsn dan tdma */
char Level[2];
char TimeStamp1[4], TimeStamp2[4], TimeStamp3[4];
int Angka[5];
char datasend[32], Sched2[3];
char datarecv[32];
char AlamatSendiri[5];
char AlamatTerima[5], LevelCheck[2];
int Sched, LevelClear;
long akk, akb;

/* variabel mqtt-sn */
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
byte datasensorrecv[7];
byte dataack[5];

void setup() {
  // put your setup code here, to run once:  
  for (int x=0; x<4; x++){
    AlamatSendiri[x] = '1'; 
    }
  Level[0] = '0';
  Serial.begin(9600);
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init(); 
  Mirf.setRADDR((byte *)AlamatSendiri);  
  Mirf.payload = 32;
  Mirf.channel = 101;
  Mirf.config(); 
  Sched = 0;
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Mirf.dataReady()){
    Mirf.getData((byte*)&datarecv);
    LevelCheck[0] = datarecv[8];
    LevelClear = strtoul(LevelCheck,NULL,0);
    if(datarecv[0] == 'S' && LevelClear < 2){
      waktu();
      sprintf(TimeStamp2,"%d",milidetik);
      TimeStampInt = milidetik;
      Serial.println(datarecv);
      synchronize();      
      }
    else{
      TDMAack();
    }
  }
  else{
    if(millis()<10){
      discovery();
      delay(100);
    }
    else if(milidetik%999==0){
      cetakwaktu();
      delay(1);
    }
  }
  waktu();
}

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
  //delay(100);
}

void discovery(){
        Mirf.setTADDR((byte *)"node");
        strcpy(datasend,"D"); //first, adding header character
        strcat(datasend,AlamatSendiri); //concat 2 var
        strcat(datasend,Level);
        Mirf.send((byte *)&datasend);
        while(Mirf.isSending()){
        Serial.println("Paket discovery sent!");
                //Serial.println(AlamatSendiri);
                //Serial.println(DataKirim);
                delay(2);
    }
}

void synchronize(){
  TDMAsched();
  for (int x=0; x<4; x++){
    AlamatTerima[x] = datarecv[x+1]; 
    }
  for (int x=0; x<3; x++){
    TimeStamp1[x] = datarecv[x+5]; 
    }
  Serial.print("request synchronization received from: ");
  Serial.println(AlamatTerima);  
  Mirf.setTADDR((byte *)AlamatTerima);//kirim balasan
  Serial.println(datarecv);
  //kirimwaktu();//timestamp3
  waktu();
  sprintf(milidetik2,"%d", milidetik);
  sprintf(detik2,"%d", detik);
  sprintf(menit2,"%d", menit);
  sprintf(jam2,"%d", jam);
  
  strcpy(datasend,"W"); //first, adding header character
  if(jam < 10){strcat(datasend,"0");}
  strcat(datasend,jam2);
  strcat(datasend,":");
  if(menit < 10){strcat(datasend,"0");}
  strcat(datasend,menit2);
  strcat(datasend,":");
  if(detik < 10){strcat(datasend,"0");}
  strcat(datasend,detik2);
  strcat(datasend,":");
  if(milidetik < 10){
    strcat(datasend,"0");
    strcat(datasend,"0");
    }
  else if(milidetik < 100){
    strcat(datasend,"0");
    }
  strcat(datasend,milidetik2);
  
  strcat(datasend,TimeStamp1);//timestamp1
  if(TimeStampInt<10){
    strcat(datasend,"0");
    //strcat(datasend,"0");
    }
  else if(TimeStampInt<100){
    strcat(datasend,"0");
    }
  strcat(datasend,TimeStamp2);//timestamp2
  sprintf(Sched2,"%d", Sched);
  strcat(datasend,Sched2);
  Mirf.send((byte *)&datasend);
  akb = millis();
  akb = akb-akk;
  Serial.println(akb);
  while(Mirf.isSending()){
    Serial.println("Packet synchronization sent!");
    Serial.println(datarecv);    
    }  
}
/*
void kirimwaktu(){
  waktu();
  sprintf(milidetik2,"%d", milidetik);
  sprintf(detik2,"%d", detik);
  sprintf(menit2,"%d", menit);
  sprintf(jam2,"%d", jam);
  
  strcpy(datasend,"W"); //first, adding header character
  if(jam < 10){strcat(datasend,"0");}
  strcat(datasend,jam2);
  strcat(datasend,":");
  if(menit < 10){strcat(datasend,"0");}
  strcat(datasend,menit2);
  strcat(datasend,":");
  if(detik < 10){strcat(datasend,"0");}
  strcat(datasend,detik2);
  strcat(datasend,":");
  if(milidetik < 10){
    strcat(datasend,"0");
    strcat(datasend,"0");
    }
  else if(milidetik < 100){
    strcat(datasend,"0");
    }
  strcat(datasend,milidetik2);
}
*/
void TDMAsched(){
  Sched = Sched + 10;
}

void TDMAack(){
  for (int x=0; x<4; x++){
    AlamatTerima[x] = datarecv[x+1]; 
  }     
  Mirf.getData((byte*)&datasensorrecv);
  byte msg_typ = MQTTSN_TYPE_PUBACK;
  byte topic_id = MQTTSN_TOPIC_ID_SUHU;
  byte returncode = MQTTSN_RC_ACCEPTED;
  if (datasensorrecv[3] == MQTTSN_TYPE_PUBLISH){
    if (datasensorrecv[0] == MQTTSN_FLAG_QOS_1){
      if (datasensorrecv[5] == MQTTSN_TOPIC_TYPE_PREDEFINED){
        if (datasensorrecv[1] == MQTTSN_TOPIC_ID_SUHU){
            Serial.print("suhu");
            Serial.print("#");
            Serial.println(datasensorrecv[4]);        
           }
        else if (datasensorrecv[1] == MQTTSN_TOPIC_ID_LEMBAB){
            Serial.print("lembab");
            Serial.print("#");
            Serial.println(datasensorrecv[4]);
          }
        }
      }
    }    
  dataack[0] = msg_typ; 
  dataack[1] = datasensorrecv[1]; 
  dataack[2] = datasensorrecv[2]; 
  dataack[3] = returncode; 
  Mirf.setTADDR((byte *)AlamatTerima);
  Mirf.send((byte*)&dataack);      
  delay (5);                  
}
