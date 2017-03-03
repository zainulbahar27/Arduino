// Initialize the Ethernet server library
#include <SPI.h>
#include <Ethernet.h>
int hijau = 3;
int kuning = 4;
int orange = 5;
int merah = 6;
int buzzer = 12;
const int outPin = 3; // Using FAST_IO
const int inPin = 2; // Using FAST_IO

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {0x98, 0x4F, 0xEE, 0x00, 0xDC, 0x8A };
IPAddress ip(192,168,1,177);
// (port 80 is default for HTTP):
EthernetServer server(80);
long duration, distance;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
  pinMode(outPin, OUTPUT_FAST);
  pinMode(inPin, INPUT_FAST);
  pinMode(hijau,OUTPUT);
  pinMode(kuning,OUTPUT); 
  pinMode(orange,OUTPUT);
  pinMode(merah,OUTPUT);
  pinMode(buzzer,OUTPUT);


}



  
void loop() {
  long duration, inches, cm;

  fastDigitalWrite(outPin, LOW);
  waitMicros(2);
  fastDigitalWrite(outPin, HIGH);
  waitMicros(10);
  fastDigitalWrite(outPin, LOW);

  duration = pulseIn(inPin, HIGH); 
  cm = microsecondsToCentimeters(duration);
  
  if (distance <= 5)
  {
    EthernetClient client = server.available();
    if (client){
      digitalWrite(hijau,LOW);
      digitalWrite(kuning,LOW);
      digitalWrite(orange,LOW);
      digitalWrite(merah,HIGH);
      digitalWrite(buzzer,HIGH);
      delay(200);
      digitalWrite(buzzer,LOW);
      delay(70);
      Serial.println("new client");
      // an http request ends with a blank line
      boolean currentLineIsBlank = true;
      while (client.connected()) {     
          if (client.available()) 
          {
            
            char c = client.read();
            Serial.write(c);
            // if you've gotten to the end of the line (received a newline
            // character) and the line is blank, the http request has ended,
            // so you can send a reply
            if (c == '\n' && currentLineIsBlank) {
               // send a standard http response header            
               client.println("<html>"); 
               client.println("<meta http-equiv=\"refresh\" content=\"0.1\">");
               client.print("<h2>");
               client.println("bahaya!!! segera evakuasi <br />");       
               client.print("jarak:");
               client.println(cm);       
               client.println("<br />");       
               client.println("</html>"); 
               break;   
            }else if (c == '\n') {
              // you're starting a new line
              currentLineIsBlank = true;
            }else if (c != '\r') {
              // you've gotten a character on the current line
              currentLineIsBlank = false;
            }
          }
      }
      // give the web browser time to receive the data
       delay(1);
      // close the connection:
      client.stop();
      Serial.println("client disonnected");
    }

  }else if(distance > 5 && distance < 10)
  {
    digitalWrite(hijau,LOW);
    digitalWrite(kuning,LOW);
    digitalWrite(orange,HIGH);
    digitalWrite(merah,LOW);
    digitalWrite(buzzer,HIGH);
    delay(200);
    digitalWrite(buzzer,LOW);
    delay(1500);
    EthernetClient client = server.available();
    if (client){
      Serial.println("new client");
      // an http request ends with a blank line
      boolean currentLineIsBlank = true;
      while (client.connected()) {     
          if (client.available()) 
          {
            
            char c = client.read();
            Serial.write(c);
            // if you've gotten to the end of the line (received a newline
            // character) and the line is blank, the http request has ended,
            // so you can send a reply
            if (c == '\n' && currentLineIsBlank) {
               // send a standard http response header            
               client.println("<html>"); 
               client.println("<meta http-equiv=\"refresh\" content=\"0.1\">");
               client.print("<h2>");
               client.println("Siaga!!! <br />");       
               client.print("jarak:");
               client.println(cm);       
               client.println("<br />");       
               client.println("</html>"); 
               break;   
            }else if (c == '\n') {
              // you're starting a new line
              currentLineIsBlank = true;
            }else if (c != '\r') {
              // you've gotten a character on the current line
              currentLineIsBlank = false;
            }
          }
      }
      // give the web browser time to receive the data
       delay(1);
      // close the connection:
      client.stop();
      Serial.println("client disonnected");
    }
  }else if(distance > 10 && distance < 15)
  {
    digitalWrite(hijau,LOW);
    digitalWrite(kuning,HIGH);
    digitalWrite(orange,LOW);
    digitalWrite(merah,LOW);
    digitalWrite(buzzer,HIGH);
    delay(200);
    digitalWrite(buzzer,LOW);
    delay(2200);
    EthernetClient client = server.available();
    if (client){
      Serial.println("new client");
      // an http request ends with a blank line
      boolean currentLineIsBlank = true;
      while (client.connected()) {     
          if (client.available()) 
          {
            
            char c = client.read();
            Serial.write(c);
            // if you've gotten to the end of the line (received a newline
            // character) and the line is blank, the http request has ended,
            // so you can send a reply
            if (c == '\n' && currentLineIsBlank) {
               // send a standard http response header            
               client.println("<html>"); 
               client.println("<meta http-equiv=\"refresh\" content=\"0.1\">");
               client.print("<h2>");
               client.println("Waspada!!! <br />");       
               client.print("jarak:");
               client.println(cm);       
               client.println("<br />");       
               client.println("</html>"); 
               break;   
            }else if (c == '\n') {
              // you're starting a new line
              currentLineIsBlank = true;
            }else if (c != '\r') {
              // you've gotten a character on the current line
              currentLineIsBlank = false;
            }
          }
      }
      // give the web browser time to receive the data
       delay(1);
      // close the connection:
      client.stop();
      Serial.println("client disonnected");
    }
  }else if(distance > 15)
  {
    digitalWrite(hijau,HIGH);
    digitalWrite(kuning,LOW);
    digitalWrite(orange,LOW);
    digitalWrite(merah,LOW);
    EthernetClient client = server.available();
    if (client){
      Serial.println("new client");
      // an http request ends with a blank line
      boolean currentLineIsBlank = true;
      while (client.connected()) {     
          if (client.available()) 
          {
            
            char c = client.read();
            Serial.write(c);
            // if you've gotten to the end of the line (received a newline
            // character) and the line is blank, the http request has ended,
            // so you can send a reply
            if (c == '\n' && currentLineIsBlank) {
               // send a standard http response header            
               client.println("<html>"); 
               client.println("<meta http-equiv=\"refresh\" content=\"0.1\">");
               client.print("<h2>");
               client.println("Air kondisi aman <br />");       
               client.print("jarak:");
               client.println(cm);       
               client.println("<br />");       
               client.println("</html>"); 
               break;   
            }else if (c == '\n') {
              // you're starting a new line
              currentLineIsBlank = true;
            }else if (c != '\r') {
              // you've gotten a character on the current line
              currentLineIsBlank = false;
            }
          }
      }
      // give the web browser time to receive the data
       delay(1);
      // close the connection:
      client.stop();
      Serial.println("client disonnected");
    }
  }
  
  
}

void waitMicros(int val)
{
  unsigned long a = micros();
  unsigned long b = micros();
  while((b-a) < val)
  {
    b = micros();
    if(a>b)
    {
      break;
    }
  }
}

long microsecondsToInches(long microseconds)
{
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds)
{
  return microseconds / 29 / 2;
}


