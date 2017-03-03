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
char data = '0';                //Variable for storing received data
int a,b,c,d,e;
int jempol,telunjuk,tengah,manis,kelingking;
float yaw,pitch,roll;
int t= 20;
int te = -20;
bool hasil(int a1, int a2, int b1, int b2, int c1, int c2,
           int d1, int d2, int e1, int e2)
{
  if (jempol <= a1+t && jempol  >= a2+te && telunjuk <= b1+t && telunjuk  >=
  b2+te && tengah  <= c1+t && tengah  >= c2+te && manis <= d1+t && manis  >= d2+te &&
  kelingking <= e1+t && kelingking  >= e2+te)
  {
   return true;
  }
  else 
   return false;
}
void kirim(bool x, char y)
{
  if(x)
  {
    Serial.print(y);
  }
}
void setup() 
{

    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
        TWBR = 24; // 400kHz I2C clock (200kHz if CPU is 8MHz)
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire:setup(400, true);
    #endif
    Serial.begin(9600);
    //while (!Serial); // wait for Leonardo enumeration, others continue immediately
    //Serial.println(F("Initializing I2C devices..."));
    mpu.initialize();
    //Serial.println(F("Testing device connections..."));
    //Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));
    //Serial.println(F("\nSend any character to start program: "));
    //while (Serial.available() && Serial.read()); // empty buffer
    //while (!Serial.available());                 // wait for data
    //while (Serial.available() && Serial.read()); // empty buffer again
    //Serial.println(F("Initializing DMP..."));
    devStatus = mpu.dmpInitialize();
    mpu.setXGyroOffset(220);
    mpu.setYGyroOffset(76);
    mpu.setZGyroOffset(-85);
    mpu.setZAccelOffset(1788); // 1688 factory default for my test chip
    if (devStatus == 0) {
        //Serial.println(F("Enabling DMP..."));
        mpu.setDMPEnabled(true);
        //Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
        attachInterrupt(0, dmpDataReady, RISING);
        mpuIntStatus = mpu.getIntStatus();
        //Serial.println(F("DMP ready! Waiting for first interrupt..."));
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
  if(Serial.available() > 0)
  {
    data = '*';                    
        while(1 && data != '#')      
        {
           if(Serial.available() > 0)
           { 
             data = Serial.read();
           }      
           if (!dmpReady) return;
            while (!mpuInterrupt && fifoCount < packetSize) {
            }
            mpuInterrupt = false;
            mpuIntStatus = mpu.getIntStatus();
            fifoCount = mpu.getFIFOCount();
            if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
                mpu.resetFIFO();
                //Serial.println(F("FIFO overflow!"));
            } else if (mpuIntStatus & 0x02) {
                while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();
                mpu.getFIFOBytes(fifoBuffer, packetSize);
                fifoCount -= packetSize; 
                #ifdef OUTPUT_READABLE_YAWPITCHROLL
                    // display Euler angles in degrees
                    mpu.dmpGetQuaternion(&q, fifoBuffer);
                    mpu.dmpGetGravity(&gravity, &q);
                    mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
                   /* Serial.print("ypr\t");
                    Serial.print(ypr[0] * 180/M_PI);
                    Serial.print("\t");
                    Serial.print(ypr[1] * 180/M_PI);
                    Serial.print("\t");
                    Serial.println(ypr[2] * 180/M_PI); */
                    a=analogRead(A0);
                    b=analogRead(A1);
                    c=analogRead(A2);
                    d=analogRead(A3);
                    e=analogRead(A7);
                    jempol = e;
                    telunjuk = b;
                    tengah = c;
                    manis = a;
                    kelingking = d; /*
                 Serial.print("1 : ");
                   Serial.println(analogRead(A7));
                   Serial.print("5 : ");
                   Serial.println(analogRead(A3));
                   Serial.print("3 : ");
                   Serial.println(analogRead(A2));
                   Serial.print("2 : ");
                   Serial.println(analogRead(A1));
                   Serial.print("4 : ");
                   Serial.println(analogRead(A0));*/
                    yaw = ypr[0] * 180/M_PI;
                    pitch = ypr[1] * 180/M_PI;
                    roll = ypr[2] * 180/M_PI;
                  
                #endif
                blinkState = !blinkState;
                digitalWrite(LED_PIN, blinkState);
                
            }
               kirim(hasil(424,424,416,416,436,436,526,526,490,490),'L');
               kirim(hasil(455,455,414,414,414,414,500,500,470,470),'U');
               kirim(hasil(489,489,435,435,431,431,497,497,488,488),'S');
               kirim(hasil(458,458,429,429,414,414,403,403,390,390),'F');
               kirim(hasil(434,434,440,440,433,433,503,503,477,477),'A');
               kirim(hasil(465,465,440,440,423,423,496,496,437,437),'T');
               kirim(hasil(515,515,414,414,429,429,517,517,481,481),'1');   
               //Serial.println("coba");
               delay(200);
        }
  }
}
