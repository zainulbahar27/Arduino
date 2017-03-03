/* Pins:
 * Hardware SPI:
 * MISO -> 12
 * MOSI -> 11
 * SCK -> 13
 *
 * Configurable:
 * CE -> 8
 * CSN -> 7*/
//melakukan pengambilan library tambahan untuk program
#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>

//melakukan deklarasi payload_length dengan panjang 10
const char payload_length =10;
//melakukan deklarasi variable datareceive untuk melakukan penerimaan data
byte datareceive[payload_length];

//fungsi untuk melakukan deklarasi variable pada program
void setup(){
  //deklarasi nilai serial dengan nilai 9600
  Serial.begin(9600);
  /*
   * Set the SPI Driver.
   */
  Mirf.spi = &MirfHardwareSpi;
  
  /*
   * Setup pins / SPI.
   */
   
  Mirf.init();
  
  /*
   * Configure reciving address.
   */
//melakukan setting RADDR dengan nama “kelserver” 
  Mirf.setRADDR((byte *)"kelserver");
  
  /*
   * Set the payload length to sizeof(unsigned long) the
   * return type of millis().
   *
   * NB: payload on client and server must be the same.
   */
  
  Mirf.payload = payload_length;
  
  /*
   * Write channel and payload config then power up reciver.
   */
    Mirf.channel = 103;
   
  //memanggil fungsi config untuk mirf
  Mirf.config();
  //mencetak kata Beginning
   Serial.println("Beginning ... "); 
 }

//fungsi yang akan berjalan secara terus menerus
void loop(){ 
  
  /*
   * If a packet has been recived.
   *
   * isSending also restores listening mode when it 
   * transitions from true to false.
   */
   
if(!Mirf.isSending() && Mirf.dataReady()){
//melakukan pengambilan data dan disimpan di variable datareceive
Mirf.getData(datareceive);
//melakukan setting TADDR dengan nama “kelclient”
Mirf.setTADDR((byte *)"kelclient");


//Melakukan delay sebesar 1000 ms
delay(1000);
//mendeklarasikan data dan mengisinya dengan panjang data yang diterima
int data = sizeof(datareceive);
//deklarasi variable i
char i;
//melakukan perulangan sebanyak data yang diterima
for (i=0;i<data;i++){
//melakukan pencetakan data yang diterima
Serial.print((char)datareceive[i]); 
}
//melakukan enter
Serial.println();
//Melakukan inisialisasi variable data dengan panjang 10 dan diisi dengan “Hai”
char time [10] = "Hai";
//melakukan set pada TADDR dengan kelclient
Mirf.setTADDR((byte *)"kelclient"); 
//melakukan send data time ke server
Mirf.send((byte *)time);
//melakukan delay selama 1000 ms
delay(1000);
}
  }


