char input;
String a;
char cmd[256],nama[50],nim[50],db[50],tb[50],x[50];
void setup()
{
  system("telnetd -l /bin/sh"); //Start the telnet server on Galileo
  system("ifconfig eth0 169.254.1.1 netmask 255.255.0.0 up");
 while (!Serial);
 
Serial.begin(9600);
}

void loop() 
{
  delay(1000);
Serial.println("Ready to execute SQL");
Serial.println("0 : membuat file database");
Serial.println("a : Menampilkan seluruh database");
Serial.println("b : Menampilkan list tabel");
Serial.println("c : Menampilkan data dari tabel");
Serial.println("d : Menambah tabel"); //-> nama tabel, nama kolom, dan type data masukan dari serial monitor
Serial.println("e : Drop tabel");  //-> tabel di tentukan dari serial monitor
Serial.println("f : Menambah data"); 
Serial.println("g : menghapus data"); //-> data referensi di tentukan dari serial monitor
Serial.println("h : mengupdate data"); //-> kolom,data referensi dan data baru  di tentukan dari serial monitor
Serial.println("c : Menampilkan data dari 2 tabel"); //-> tabel di tentukan dari serial moniitor
Serial.print("masukan pilihan : ");
while(1)
{
  if(Serial.available()>0)
  {
    input = Serial.read();
    if (input == '0')
    {
      Serial.print("masukan nama database yang akan di buat : ");
      while(!Serial.available()) ;
      a = Serial.readString();
      a.toCharArray(x,a.length()+1);
      Serial.println(x);
      sprintf(cmd,"touch /home/root/\"%s\"  &> /dev/ttyGS0", x);
     system(cmd);
    }
    else if (input == 'a')
    {
       system ("ls /home/root/ &> /dev/ttyGS0 ");
      Serial.println("Pilih database yang digunakan : ");
      Serial.print("tulis lengkap- ");
      while(!Serial.available()) ;
      a = Serial.readString();
      a.toCharArray(db,a.length()+1);
      Serial.println(db);
    }
    

    else if (input == 'b')
    {
     Serial.println();
     sprintf(cmd,"sqlite3 -csv -header /home/root/\"%s\" .table &> /dev/ttyGS0", db);
     system(cmd);
     Serial.println("Pilih tabel yang digunakan : ");
     Serial.print("tulis lengkap- ");
     while(!Serial.available()) ;
      a = Serial.readString();
      a.toCharArray(tb,a.length()+1);
      Serial.println(tb);
    }
    
    else if (input == 'c')
    {
    
     sprintf(cmd,"sqlite3 -csv -header /home/root/\"%s\" 'select * from \"%s\";' &> /dev/ttyGS0", db, tb);
     system(cmd);
    }
    
    else if (input == 'f')
    {
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
        
      sprintf(cmd,"sqlite3 -csv -header /home/root/\"%s\" 'insert into \"%s\" VALUES(\"%s\",\"%s\");' &> /dev/ttyGS0", db, tb, nama, nim);
      system(cmd);
    }
    else if (input == 'g')
    {
      
    }
    else
    {
      Serial.println("Masukan dengan benar");
    }
    break;
  }
}

  
}

