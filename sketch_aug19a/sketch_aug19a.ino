#include <Wire.h>
#include "RTClib.h"
#include "DHT.h"

RTC_DS1307 RTC;
int soilPin = A0;
#define DHTPIN 7 
int thresholdValue = 800;
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

const int S_IDLE = 1;
const int S_SENSING = 2;
const int S_SEND = 3;
  static int state = S_IDLE; // initial state is 1, the "idle" state.
  static unsigned long ts;  // To store the "current" time in for delays.


void setup()
{
  Serial.begin(9600);
  Wire.begin();
  RTC.begin();
  RTC.adjust(DateTime(__DATE__, __TIME__)); //jam menyesuaikan computer
  pinMode(soilPin, INPUT);
  dht.begin();

}

void loop()
{

  switch(state)
  {
    case S_IDLE:{
      // We don't need to do anything here, waiting for a forced state change.
      break;
    }
    case S_SENSING:{

 DateTime now = RTC.now();
 //We print the day
 Serial.print(now.day(), DEC);
 Serial.print('/');
 //We print the month
 Serial.print(now.month(), DEC);
 Serial.print('/');
 //We print the year
 Serial.print(now.year(), DEC);
 Serial.print(' ');
 //We print the hour
 Serial.print(now.hour(), DEC);
 Serial.print(':');
 //We print the minutes
 Serial.print(now.minute(), DEC);
 Serial.print(':');
 //We print the seconds
 Serial.print(now.second(), DEC);
 Serial.println();
 delay(500); // selama 500 mili second
 
 int sensorValue = analogRead(soilPin);//mebaca pin analog sensor tanah
  Serial.print(sensorValue);
    if(sensorValue < thresholdValue){
      Serial.print(" - Doesn't need watering \t ");

  }
  else {
      Serial.print(" - Time to water your plant \t");
      delay(500);
    
     // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
 
  // Check if any reads failed and exit early (to try again).
  
  Serial.print("Kelembaban Udara: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Suhu Udara: ");
  Serial.print(t);
  Serial.print(" *C ");
    delay(500);
}
      break;
    }
    case S_SEND : {
      // If one second has passed, then move on to the next state.
      if(millis() > ts + 1000)
      {
        state = S_IDLE;
      } 
      break;
    }
    default :{
      state = S_IDLE;
      break;
  }
  }
}
