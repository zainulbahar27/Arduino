#include <string.h>

String a;
char cmd[256],nama[50],nim[50];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
    system("telnetd -l /bin/sh"); //Start the telnet server on Galileo
  system("ifconfig eth0 169.254.1.1 netmask 255.255.0.0 up");

}

void loop() {
  // put your main code here, to run repeatedly:

  Serial.println("Insert nama: ");
  while(!Serial.available()) ;
  a = Serial.readString();
  a.toCharArray(nama,a.length()+1);
  Serial.println(nama);

  Serial.println("Insert nim: ");
  while(!Serial.available()) ;
  a = Serial.readString();
  a.toCharArray(nim,a.length()+1);
  Serial.println(nim);
  
  sprintf(cmd,"sqlite3 -csv -header /home/root/test1.db 'insert into mhs VALUES(\"%s\",\"%s\")' &> /dev/ttyGS0", nama, nim);
  system(cmd);
}
