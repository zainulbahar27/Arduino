#include <DistanceGP2Y0A41SK.h>

DistanceGP2Y0A41SK Dist;
int distance;

void setup()
{
  Serial.begin(9600);
  Dist.begin(A0);
}

void loop()
{
  distance = Dist.getDistanceCentimeter();
  Serial.print("\nDistance in centimeters: ");
  Serial.print(distance);  
  delay(500); //make it readable
}