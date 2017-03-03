int sensorpin = 0;                 // analog pin used to connect the sharp sensor
int val = 0;                 // variable to store the values from sensor(initially zero)

void setup()
{
  Serial.begin(9600);               // starts the serial monitor
}
 
void loop()
{
  val = analogRead(sensorpin); 
  float volt = val/51;
  float jarak=((1/((0.04*volt)-0.006)-0.42));
  Serial.println(jarak);
  delay(100);                    
}
