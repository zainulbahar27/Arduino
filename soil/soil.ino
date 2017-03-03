int led1 =0;
int led2 =1;
int led3 =2;
int led4 =3;
int led5 =4;
int probe =5;
int mostureSensor = A0;
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
pinMode(led1, OUTPUT);
pinMode(led2, OUTPUT);
pinMode(led3, OUTPUT);
pinMode(led4, OUTPUT);
pinMode(led5, OUTPUT);
pinMode(probe, OUTPUT);
}

// the loop routine runs over and over again forever:

void loop() {

// Power-up the probe and pause for the driver

digitalWrite(probe, HIGH);

delay(1000);

// read the input on analog pin 0:

int sensorValue = analogRead(mostureSensor);

// print out the value you read:

Serial.println(sensorValue);

if (sensorValue >= 820)

{
digitalWrite(led1, HIGH);

digitalWrite(led2, HIGH);

digitalWrite(led3, HIGH);

digitalWrite(led4, HIGH);

digitalWrite(led5, HIGH);

}

else if (sensorValue >= 615 && sensorValue < 820)

{

digitalWrite(led1, HIGH);

digitalWrite(led2, HIGH);

digitalWrite(led3, HIGH);

digitalWrite(led4, HIGH);

digitalWrite(led5, LOW);

}

else if (sensorValue >= 410 && sensorValue < 615)

{

digitalWrite(led1, HIGH);

digitalWrite(led2, HIGH);

digitalWrite(led3, HIGH);

digitalWrite(led4, LOW);

digitalWrite(led5, LOW);

}

else if (sensorValue >= 250 && sensorValue < 410)

{

digitalWrite(led1, HIGH);

digitalWrite(led2, HIGH);

digitalWrite(led3, LOW);

digitalWrite(led4, LOW);

digitalWrite(led5, LOW);

}

else if (sensorValue >= 0 && sensorValue < 250)

{

digitalWrite(led1, HIGH);

digitalWrite(led2, LOW);

digitalWrite(led3, LOW);

digitalWrite(led4, LOW);

digitalWrite(led5, LOW);

}

// Power-down the probe

digitalWrite(probe, LOW);

delay(500); // wait half an hour

}
