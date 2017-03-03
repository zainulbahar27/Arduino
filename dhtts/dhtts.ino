// Project Name: Gardening Arduino
// Start: 02/09/2012
// Started by Dewo, setio.dewo@gmail.com, https://dewo.wordpress.com
 
#include <dht.h>
 
dht DHT;
#define DHT_pin 8
#define DHT_jeda 5000
#define DHT_max_temp 27
#define LED_pin 12
#define LED_jeda 500
 
unsigned long DHT_millis = 0;
unsigned long LED_millis = 0;
boolean _blink = false;
boolean LED_state = LOW;
 
void setup() {
  Serial.begin(9600);
  pinMode(LED_pin, OUTPUT);
}
 
void loop() {
  DHT_read();
  LED_blink();
}
 
void LED_blink() {
  unsigned long LED_current = millis();
  if (_blink) {
    if (LED_current - LED_millis > LED_jeda) {
      LED_millis = LED_current;
      if (LED_state == LOW) LED_state = HIGH;
      else LED_state = LOW;
    }
    digitalWrite(LED_pin, LED_state);
  }
  else digitalWrite(LED_pin, LOW);
}
 
void DHT_read() {
  unsigned long DHT_current = millis();
  if (DHT_current - DHT_millis >= DHT_jeda) {
    DHT_millis = DHT_current;
    int DHT_chk = DHT.read(DHT_pin);
    switch(DHT_chk) {
      case DHTLIB_OK:
        Serial.println("OK");
        break;
      case DHTLIB_ERROR_CHECKSUM:
        Serial.println("Checksum error");
        break;
      case DHTLIB_ERROR_TIMEOUT:
        Serial.println("Time out error");
        break;
      default:
        Serial.println("Unknown error");
        break;
    }
    // Apakah melebihi batas yg ditentukan?
    String alert_temp = "";
    // (>27)
    if (DHT.temperature >= DHT_max_temp) {
      _blink = true;
      alert_temp.concat("(*>=");
      alert_temp.concat((String)DHT_max_temp);
      alert_temp.concat(")");
    }
    else _blink = false;
    // Kirimkan nilai ke serial
    Serial.print("Suhu: ");
    Serial.print((float)DHT.temperature, 2);
    Serial.println(alert_temp);
    Serial.print("Kelembaban: ");
    Serial.println((float)DHT.humidity, 2);
    Serial.print("\n");
  }
}
