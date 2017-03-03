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
char data = 0;                //Variable for storing received data
int a, b, c, d, e;
void setup()
{
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
  Wire.begin();
  TWBR = 24; // 400kHz I2C clock (200kHz if CPU is 8MHz)
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
  Fastwire::setup(400, true);
#endif
  Serial.begin(9600);
  while (!Serial); // wait for Leonardo enumeration, others continue immediately
  Serial.println(F("Initializing I2C devices..."));
  mpu.initialize();
  Serial.println(F("Testing device connections..."));
  Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));
  Serial.println(F("\nSend any character to start program: "));
  while (Serial.available() && Serial.read()); // empty buffer
  while (!Serial.available());                 // wait for data
  while (Serial.available() && Serial.read()); // empty buffer again
  Serial.println(F("Initializing DMP..."));
  devStatus = mpu.dmpInitialize();
    mpu.setXGyroOffset(40);
    mpu.setYGyroOffset(-16);
    mpu.setZGyroOffset(-20);
    mpu.setXAccelOffset(-3404);
    mpu.setYAccelOffset(-2183);
    mpu.setZAccelOffset(1762);
 if (devStatus == 0) {
    Serial.println(F("Enabling DMP..."));
    mpu.setDMPEnabled(true);
    Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
    attachInterrupt(0, dmpDataReady, RISING);
    mpuIntStatus = mpu.getIntStatus();
    Serial.println(F("DMP ready! Waiting for first interrupt..."));
    dmpReady = true;
    packetSize = mpu.dmpGetFIFOPacketSize();
  } else {
    Serial.print(F("DMP Initialization failed (code "));
    Serial.print(devStatus);
    Serial.println(F(")"));
  }

  // configure LED for output
  pinMode(LED_PIN, OUTPUT);

}
void loop()
{
  Serial.println("1. mode sleep ");
  Serial.println("2. mede running ");
  Serial.print("Masukan pilihan anda : ");
  while (1)
  {
    if (Serial.available() > 0)
    {
      data = Serial.read();
      Serial.println(data);
      if (data == '1') {
        Serial.println("Masukan 0 untuk kembali");
        char input = '1';
        while (input != '0')
        {
          if (Serial.available() > 0)
          {
            input = Serial.read();
            Serial.println(input);

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
              Serial.println("ypr\t");
              Serial.print(ypr[0] * 180 / M_PI);
              Serial.print("\t");
              Serial.print(ypr[1] * 180 / M_PI);
              Serial.print("\t");
              Serial.println(ypr[2] * 180 / M_PI);
              Serial.print("Jempol : ");
              Serial.print(analogRead(A7));
              Serial.print("\t");
              Serial.print("Kelingking : ");
              Serial.print(analogRead(A3));
              Serial.print("\t");
              Serial.print("Tengah : ");
              Serial.print(analogRead(A2));
              Serial.print("\t");
              Serial.print("Telunjuk : ");
              Serial.print(analogRead(A1));
              Serial.print("\t");
              Serial.print("Manis : ");
              Serial.println(analogRead(A0));
#endif
              blinkState = !blinkState;
              digitalWrite(LED_PIN, blinkState);
            }
          }
        }

      }
      else if (data == '2')
      {
        int n = 0;
        int nx = 0;
        Serial.println("Masukan 0 untuk kembali");
        char input = '1';
        while (input != '0')
        {
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

#endif
            blinkState = !blinkState;
            digitalWrite(LED_PIN, blinkState);
          }
          if (Serial.available() > 0)
          {
            n++;
            input = Serial.read();
            if(n%2!=0)
            {
             nx++;
            Serial.println();
            Serial.println(input);
            Serial.print(nx);
            Serial.print(". data :");
            }
            Serial.print("\t");
            Serial.print(analogRead(A7));
            Serial.print("\t");
//            Serial.print("Kelingking : ");
            Serial.print(analogRead(A3));
            Serial.print("\t");
//            Serial.print("Tengah : ");
            Serial.print(analogRead(A2));
            Serial.print("\t");
//            Serial.print("Telunjuk : ");
            Serial.print(analogRead(A1));
            Serial.print("\t");
//            Serial.print("Manis : ");
            Serial.print(analogRead(A0));
            Serial.print("\t");
            Serial.print(ypr[0] * 180 / M_PI);
            Serial.print("\t");
            Serial.print(ypr[1] * 180 / M_PI);
            Serial.print("\t");
            Serial.print(ypr[2] * 180 / M_PI);
          }
        }
      }
      else
      {
        Serial.println("Masukan pilihan dengan benar");
      }
      break;
    }
  }

}
