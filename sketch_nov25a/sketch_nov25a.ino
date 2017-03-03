
char nama[15] ;

String a;
char cmd[256];


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  system("telnetd -l /bin/sh"); //Start the telnet server on Galileo
  system("ifconfig eth0 169.254.1.1 netmask 255.255.0.0 up");
}

void loop() {
  // put your main code here, to run repeatedly:

  Serial.print("Insert nama: ");
  while (!Serial.available()) ;
  a = Serial.readString();
  a.toCharArray(nama, a.length() + 1);
  Serial.println(nama);

  sprintf(cmd, "sqlite3 -csv -header /home/root/test1.db 'insert into tbl1 VALUES(\"%s\",8);' &> /dev/ttyGS0", nama);
  //untuk membuat file gunakan perintah touch
  //untuk menghapus gunakan perintah rm
  system(cmd);
  
}

