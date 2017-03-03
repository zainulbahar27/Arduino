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
uint8_t teapotPacket[14] = { '$', 0x02, 0,0, 0,0, 0,0, 0,0, 0x00, 0x00, '\r', '\n' };
volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
    mpuInterrupt = true;
}
char data = 0;                //Variable for storing received data
int a,b,c,d,e;
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
    mpu.setXGyroOffset(220);
    mpu.setYGyroOffset(76);
    mpu.setZGyroOffset(-85);
    mpu.setZAccelOffset(1788); // 1688 factory default for my test chip
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
  Serial.println("Silahkan Pilih menu");
  Serial.println("1. Baca Sensor MPU-6050");
  Serial.println("2. Baca Seluruh flex sensor");
  Serial.println("3. Baca data terakhir sensor");
  Serial.print("Masukan pilihan anda : "); 
  while(1)
  {
    delay(500);
  if(Serial.available() > 0)
  {
    data = Serial.read();      
    Serial.print(data);        
    Serial.print("\n");         
    if(data == '1'){
      Serial.println("Masukan 0 untuk kembali");
       char input ='1';
        while(input !='0')      
        {
           if(Serial.available() > 0)
           { 
             input = Serial.read();
           }      
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
      Serial.println("================================");
      Serial.println("Nilai flex sensor terakhir");
      Serial.print("A7/A3/A2/A1/A0 : \t");
      Serial.print(analogRead(A7));
      Serial.print("\t");
      Serial.print(analogRead(A3));
      Serial.print("\t");
      Serial.print(analogRead(A2));
      Serial.print("\t");
      Serial.print(analogRead(A1));
      Serial.print("\t");
      Serial.println(analogRead(A0));
      Serial.println("================================");
      Serial.println("Nilai Yaw Pitch Roll terakhir");
      Serial.print("ypr\t");
      Serial.print(ypr[0] * 180/M_PI);
      Serial.print("\t");
      Serial.print(ypr[1] * 180/M_PI);
      Serial.print("\t");
      Serial.println(ypr[2] * 180/M_PI);
      Serial.println("================================");
                #endif
                blinkState = !blinkState;
                digitalWrite(LED_PIN, blinkState);
            }
        }

    }
    else if(data == '2')
    {
      Serial.println("Masukan 0 untuk kembali");
      char input = '1';
        while(input != '0')      
        {
           delay(200);
           if(Serial.available() > 0)
           { 
             input = Serial.read();
           }
           delay(1000);
           Serial.print("A7 : ");
           Serial.println(analogRead(A7));
           Serial.print("A3 : ");
           Serial.println(analogRead(A3));
           Serial.print("A2 : ");
           Serial.println(analogRead(A2));
           Serial.print("A1 : ");
           Serial.println(analogRead(A1));
           Serial.print("A0 : ");
           Serial.println(analogRead(A0));
           a=analogRead(A0);
           b=analogRead(A1);
           c=analogRead(A2);
           d=analogRead(A3);
           e=analogRead(A7);
        }
    }
    else if(data == '3')
    {
      Serial.println("================================");
      Serial.println("Nilai flex sensor terakhir");
      Serial.print("A7/A3/A2/A1/A0 : \t");
      Serial.println(analogRead(A7));
      Serial.print("\t");
      Serial.println(analogRead(A3));
      Serial.print("\t");
      Serial.println(analogRead(A2));
      Serial.print("\t");
      Serial.println(analogRead(A1));
      Serial.print("\t");
      Serial.println(analogRead(A0));
      Serial.println("================================");
      Serial.println("Nilai Yaw Pitch Roll terakhir");
      Serial.print("ypr\t");
      Serial.print(ypr[0] * 180/M_PI);
      Serial.print("\t");
      Serial.print(ypr[1] * 180/M_PI);
      Serial.print("\t");
      Serial.println(ypr[2] * 180/M_PI);
      Serial.println("================================");
    }
    else 
    {
      Serial.println("Masukan pilihan dengan benar");
    }
      break;
  }
  }  
 
}
