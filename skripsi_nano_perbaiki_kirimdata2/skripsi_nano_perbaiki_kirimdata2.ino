#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 9);
#include <SPI.h>
#include <SoftwareSerial.h>
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include "Wire.h"
#endif
MPU6050 mpu;
#define OUTPUT_READABLE_YAWPITCHROLL

#define LED_PIN 13 // (Arduino is 13, Teensy is 11, Teensy++ is 6)
bool blinkState = false;
// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer
Quaternion q;           // [w, x, y, z]         quaternion container
VectorFloat gravity;    // [x, y, z]            gravity vector
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
uint8_t teapotPacket[14] = { '$', 0x02, 0, 0, 0, 0, 0, 0, 0, 0, 0x00, 0x00, '\r', '\n' };
volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
  mpuInterrupt = true;
}
int flag = 0;
char data = '0';
double masukan[7];
double waktu;
int nomer=0;
int urut=0;
void setup() {
  // initialise serial communications at 9600 bps:
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
  Wire.begin();
  TWBR = 24; // 400kHz I2C clock (200kHz if CPU is 8MHz)
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
  Fastwire::setup(400, true);
#endif
  Serial.begin(9600);
  mySerial.begin(9600);
//    while (!Serial); // wait for Leonardo enumeration, others continue immediately
//    Serial.println(F("Initializing I2C devices..."));
  mpu.initialize();
//   Serial.println(F("Testing device connections..."));
//   Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));
//   Serial.println(F("\nSend any character to start program: "));
//   while (Serial.available() && Serial.read()); // empty buffer
//   while (!Serial.available());                 // wait for data
//   while (Serial.available() && Serial.read()); // empty buffer again
//   Serial.println(F("Initializing DMP..."));
  devStatus = mpu.dmpInitialize();
    mpu.setXGyroOffset(40);
    mpu.setYGyroOffset(-16);
    mpu.setZGyroOffset(-20);
    mpu.setXAccelOffset(-3404);
    mpu.setYAccelOffset(-2183);
    mpu.setZAccelOffset(1762);
  if (devStatus == 0) {
//    Serial.println(F("Enabling DMP..."));
    mpu.setDMPEnabled(true);
//    Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
    attachInterrupt(0, dmpDataReady, RISING);
    mpuIntStatus = mpu.getIntStatus();
//    Serial.println(F("DMP ready! Waiting for first interrupt..."));
    dmpReady = true;
    packetSize = mpu.dmpGetFIFOPacketSize();
      } else {
//        Serial.print(F("DMP Initialization failed (code "));
//       Serial.print(devStatus);
//       Serial.println(F(")"));
     }
    
     // configure LED for output
    pinMode(LED_PIN, OUTPUT);
  }

  void loop() {
    if (!dmpReady) return;
    while (!mpuInterrupt && fifoCount < packetSize) {
    }
    mpuInterrupt = false;
    mpuIntStatus = mpu.getIntStatus();
    fifoCount = mpu.getFIFOCount();
    if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
      mpu.resetFIFO();
      Serial.println(F("FIFO overflow!"));
    } else if (mpuIntStatus & 0x02) {
      while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();
      mpu.getFIFOBytes(fifoBuffer, packetSize);
      fifoCount -= packetSize;
#ifdef OUTPUT_READABLE_YAWPITCHROLL
      // display Euler angles in degrees
      mpu.dmpGetQuaternion(&q, fifoBuffer);
      mpu.dmpGetGravity(&gravity, &q);
      mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
      masukan[0]=analogRead(A7);
      masukan[1]=analogRead(A3);
      masukan[2] = analogRead(A2);
      masukan[3] = analogRead(A1);
      masukan[4] = analogRead(A0);
//      masukan[5] = (ypr[0] * 180 / M_PI);
      masukan[5] = (ypr[1] * 180 / M_PI);
      masukan[6] = (ypr[2] * 180 / M_PI);
//     
//      Serial.print(ypr[0] * 180 / M_PI);
//      Serial.print("\t");
//      Serial.print (ypr[1] * 180 / M_PI);
//      Serial.print("\t");
//      Serial.println (ypr[2] * 180 / M_PI);
#endif
      blinkState = !blinkState;
      digitalWrite(LED_PIN, blinkState);
    }
    if (mySerial.available() > 0) {
      data = mySerial.read();
      delay(20);
      Serial.println("data diterima : ");
      Serial.println(data);
    }
    if (data == '1')
    {
      nomer++;
      waktu = millis();
      sendAndroidValues();
      Serial.println("data di kirim");
      while (true)
      {
        if (mySerial.available() > 0) {
          data = mySerial.read();
          delay(20);
          Serial.println("data diterima : ");
          Serial.println(data);
          if (data == '3'||data == '1')
          {
            sendAndroidValues();
            Serial.println("data di kirim ulang");
          }
          else if (data == '2')
          {
            data = '0';
            delay(20);
            Serial.println("data berhasil di proses");
            if(nomer%2==0)
            {
              urut++;
              Serial.print("data ke =");
              Serial.print(urut);
              Serial.print("\tKomputasi=");
              Serial.println(millis()-waktu);
            }
            break;
          }
        }
        delay(10);
      }
    }
//    delay(10);
  }
  //sends the values from the sensor over serial to BT module
  void sendAndroidValues()
  {
    delay(100);        //added a delay to eliminate missed transmissions
    //puts # before the values so our app knows what to do with the data
    mySerial.print('#');
    Serial.print('#');
    //for loop cycles through 4 sensors and sends values via serial
    for (int k = 0; k < 7; k++)
    {
      Serial.print(masukan[k]);
      delay(5);
      Serial.print('\t');
      delay(5);
      mySerial.print(masukan[k]);
      delay(5);
      mySerial.print('+');
      delay(5);
      //technically not needed but I prefer to break up data values
      //so they are easier to see when debugging
    }
    Serial.print('~'); //used as an end of transmission character - used in app for string length
    Serial.println();
    mySerial.print('~'); //used as an end of transmission character - used in app for string length
    mySerial.println();
    delay(100);        //added a delay to eliminate missed transmissions
  }
