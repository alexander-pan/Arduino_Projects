/*
 Example sketch for the PS3 USB library - developed by Kristian Lauszus
 For more information visit my blog: http://blog.tkjelectronics.dk/ or
 send me an e-mail:  kristianl@tkjelectronics.com
 */

//#include <PS3USB.h>
#include <PS3BT.h>
#include <usbhub.h>
#include <Wire.h>
#include <Servo.h> 
#include <Adafruit_MotorShield.h>

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

#define PIN 2
#define PIXELS 1

//neopixels
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(PIXELS, PIN, NEO_GRB + NEO_KHZ800);

//Initialize all the USB Shield 
USB Usb;
/* You can create the instance of the class in two ways */
//PS3USB PS3(&Usb); // This will just create the instance
//PS3USB PS3(&Usb,0x00,0x15,0x83,0x3D,0x0A,0x57); // This will also store the bluetooth address - this can be obtained from the dongle when running the sketch
BTD Btd(&Usb);
PS3BT PS3(&Btd);

int servostate = 90;
int servostate2 = 0;
int servostate3 = 0;
int servostate4 = 90;
int motorspeed = 70;

//Initialize Motor Shield Items
// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_StepperMotor *stepper  = AFMS.getStepper(200,2);
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

void setup() {
  Serial.begin(115200);
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); //halt
  }
  Serial.print(F("\r\nPS3 USB Library Started"));
  //attach a servo to pin 7,5,4,3
  servo1.attach(7);
  servo2.attach(5);
  servo3.attach(4);
  servo4.attach(3);
  servo1.write(90);
  servo2.write(0);
  servo3.write(0);
  servo4.write(90);
  delay(100);
  servo1.detach();
  servo2.detach();
  servo3.detach();
  servo4.detach();

  //stepper motor setup
  AFMS.begin(); //create default frequency 1.6KHz
  stepper->setSpeed(motorspeed);

  //neopixel pixel
  pixels.begin();
  pixels.setPixelColor(0,pixels.Color(0,0,0));
  pixels.show();
}
void loop() {
  Usb.Task();

  if (PS3.PS3Connected || PS3.PS3NavigationConnected) {
    
    if (PS3.getButtonClick(PS))
      Serial.print(F("\r\nPS"));

    //Analog Controllers Left Joystick: Controls the Stepper Motor
    if (PS3.getAnalogHat(LeftHatX) < 117){
      Serial.print(F("\r\nLeftHatX: "));
      Serial.print(PS3.getAnalogHat(LeftHatX));
      stepper->step(20,BACKWARD,DOUBLE);
    }
    if (PS3.getAnalogHat(LeftHatX) > 137){
      Serial.print(F("\r\nLeftHatX: "));
      Serial.print(PS3.getAnalogHat(LeftHatX));
      stepper->step(20,FORWARD,DOUBLE);
    }

    //Activate Index Finger Individually
    if (PS3.getButtonPress(TRIANGLE)){
      //Serial.println(servostate);
      servo1.attach(7);
      if(servostate > 180)
        servostate = 180;
      else
        servostate++;
      servo1.write(servostate);
    }
    else if (~PS3.getButtonPress(TRIANGLE) && servostate > 90){
      servostate = 90;
      servo1.write(servostate);
      delay(50);
      servo1.detach();
    }

    //Activate Middle Finger Individually
    if (PS3.getButtonPress(CIRCLE)){
      //Serial.println(servostate);
      servo2.attach(5);
      if(servostate2 > 180)
        servostate2 = 180;
      else
        servostate2++;
      servo2.write(servostate2);
    }
    else if (~PS3.getButtonPress(CIRCLE) && servostate2 > 0){
      servostate2 = 0;
      servo2.write(servostate2);
      delay(50);
      servo2.detach();
    }
    
    //Activate Ring Finger Individually
    if (PS3.getButtonPress(CROSS)){
      //Serial.println(servostate);
      servo3.attach(4);
      if(servostate3 > 180)
        servostate3 = 180;
      else
        servostate3++;
      servo3.write(servostate3);
    }
    else if (~PS3.getButtonPress(CIRCLE) && servostate3 > 0){
      servostate3 = 0;
      servo3.write(servostate3);
      delay(50);
      servo3.detach();
    }

    //Activate Thumb Finger Individually
    if (PS3.getButtonPress(SQUARE)){
      //Serial.println(servostate4);
      servo4.attach(3);
      if(servostate4 > 180)
        servostate4 = 180;
      else
        servostate4++;
      servo4.write(servostate4);
    }
    else if (~PS3.getButtonPress(SQUARE) && servostate4 > 90){
      //Serial.println(servostate4);
      servostate4 = 90;
      servo4.write(servostate4);
      delay(50);
      servo4.detach();
    }

    if (PS3.getButtonClick(UP)) {
      Serial.print(F("\r\nUp"));
      motorspeed = motorspeed + 10;
      Serial.print(motorspeed);
      stepper->setSpeed(motorspeed);
    }
    if (PS3.getButtonClick(DOWN)) {
      Serial.print(F("\r\nDown"));
      motorspeed = motorspeed - 10;
      Serial.print(motorspeed);
      stepper->setSpeed(motorspeed);
    }
    if (PS3.getButtonClick(RIGHT)) {
    }
    if (PS3.getButtonClick(LEFT)) {
    }

//    if (PS3.getButtonClick(L1))
//      Serial.print(F("\r\nL1"));
//    if(PS3.getButtonClick(L2))
//      Serial.print(F("\r\nL2"));
//    if (PS3.getButtonClick(L3))
//      Serial.print(F("\r\nL3"));

    if (PS3.getButtonPress(R1)){
      //Serial.print(F("\r\nR1"));
      pixels.setPixelColor(0,pixels.Color(255,150,0));
      pixels.show();
    }
    else if (~PS3.getButtonPress(R1)){
      pixels.setPixelColor(0,pixels.Color(0,0,0));
      pixels.show();
    }
//    if(PS3.getButtonClick(R2))
//      Serial.print(F("\r\nR2"));
//    if (PS3.getButtonClick(R3))
//      Serial.print(F("\r\nR3"));

//    if (PS3.getButtonClick(SELECT)) {
//      Serial.print(F("\r\nSelect - "));
//      PS3.printStatusString();
//    }
//    if (PS3.getButtonClick(START)) {
//      Serial.print(F("\r\nStart"));
//      printAngle = !printAngle;
//    }
  }
  if(servostate > 180)
    servostate=180;
  else if (servostate < 0)
    servostate = 0;
}

