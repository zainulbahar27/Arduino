#include <SPI.h>
#include <Ethernet.h>
byte mac[] = {0x98, 0x4F, 0xEE,0x00, 0x1f, 0x60};
byte ip[] ={0,0,0,0};
void setup() {
  delay(5000);
  Serial.begin(9600);
  Serial.println("Attempting to configure Ethernet Using DHCP");
  if (Ethernet.begin(mac)==0)
  {
    Serial.println("Filed to configure Ethernet using DHCP");
    Serial.println("Attempting to configure Ethernet Using static IPP");
    Ethernet.begin(mac, ip);
    Serial.println(" release check ifconfig");
  }
  else
  Serial.println("sounds god");
  Ethernet.begin(mac, ip);
  system("ifupeth0");
  system("ifconfig eth0 192.168.0.2");
  Serial.print("galileo IP addres : ");
  Serial.println (Ethernet.localIP());
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly: 
  
}
