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

#define PIN 3
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

//Initialize Motor Shield Items
// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
//Adafruit_StepperMotor *stepper  = AFMS.getStepper(300,2);
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

//Initiate numerical ints
//int servo1,servo2,servo3,servo4,pulselength,pulselength2,pulselength3,pulselength4;
int servostate = 0;
int servostate2 = 0;
int servostate3 = 0;
int servostate4 = 0;
int servostate5 = 0;
//int motorspeed = 100;
int animation = 0;

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

  servo1.attach(7);
  servo1.write(0);

  servo2.attach(6);
  servo2.write(0);

  servo3.attach(5);
  servo3.write(-20);

  servo4.attach(4);
  servo4.write(-20);
  
  //stepper motor setup
  AFMS.begin(); //create default frequency 1.6KHz
  //stepper->setSpeed(motorspeed);
  
  //neopixel pixel
  pixels.begin();
  pixels.setPixelColor(0,pixels.Color(0,0,0));
}
void loop() {
  Usb.Task();
  
  if (PS3.PS3Connected || PS3.PS3NavigationConnected) {  
    if (PS3.getButtonClick(PS))
      Serial.print(F("\r\nPS"));
    /*
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
    */

    if(PS3.getAnalogHat(LeftHatX) < 117){
      //Serial.println(PS3.getAnalogHat(LeftHatX));
      servostate5 = map(PS3.getAnalogHat(LeftHatX),100,0,0,100);
      if(servostate5 > 100)
        servostate5 = 100;

      if(servostate5 < 0)
        servostate5=0;
      Serial.println(servostate5);
      servo1.write(servostate5);
      servo2.write(servostate5);
      servo3.write(servostate5);
      servo4.write(servostate5);
    }
    
    //Activate Index Finger Individually
    if (PS3.getButtonPress(TRIANGLE)){
      //Serial.println("Triangle");
      if(servostate > 100)
        servostate = 100;
      else
        servostate++;
      //pulselength = map(servostate,0,180,SERVOMIN,SERVOMAX);
      //Serial.println(pulselength);
      //pwm.setPWM(servo1,0,pulselength);
      servo1.write(servostate);
    }
    else if (~PS3.getButtonPress(TRIANGLE) && servostate > 0){
      servostate = 0;
      //pulselength = map(servostate,0,180,SERVOMIN,SERVOMAX);
      //Serial.println(pulselength);
      //pwm.setPWM(servo1,0,pulselength);
      servo1.write(servostate);
    }
    
    //Activate Middle Finger Individually
    if (PS3.getButtonPress(CIRCLE)){
      if(servostate2 > 100)
        servostate2 = 100;
      else
        servostate2++;
      //pulselength2 = map(servostate2,0,180,SERVOMIN,SERVOMAX);
      //pwm.setPWM(servo2,0,pulselength2);
      servo2.write(servostate2);
    }
    else if (~PS3.getButtonPress(CIRCLE) && servostate2 > 0){
      servostate2 = 0;
      //pulselength2 = map(servostate2,0,180,SERVOMIN,SERVOMAX);
      //pwm.setPWM(servo2,0,pulselength2);
      servo2.write(servostate2);
    }
    
    //Activate Ring Finger Individually
    if (PS3.getButtonPress(CROSS)){
      if(servostate3 > 100)
        servostate3 = 100;
      else
        servostate3++;
      //pulselength3 = map(servostate3,0,180,SERVOMIN,SERVOMAX);
      //pwm.setPWM(servo3,0,pulselength3);
      servo3.write(servostate3);
    }
    else if (~PS3.getButtonPress(CROSS) && servostate3 > -20){
      servostate3 = -20;
      //pulselength3 = map(servostate3,0,180,SERVOMIN,SERVOMAX);
      //pwm.setPWM(servo3,0,pulselength3);
      servo3.write(servostate3);
    }

    //Activate Thumb Finger Individually
    if (PS3.getButtonPress(SQUARE)){
      Serial.println("SQUARE");
      if(servostate4 > 100)
        servostate4 = 100;
      else
        servostate4++;
      //pulselength4 = map(servostate4,0,180,SERVOMIN,SERVOMAX);
      //pwm.setPWM(servo4,0,pulselength4);
      servo4.write(servostate4);
    }
    else if (~PS3.getButtonPress(SQUARE) && servostate4 > -20){
      servostate4 = -20;
      //pulselength4 = map(servostate4,0,180,SERVOMIN,SERVOMAX);
      //pwm.setPWM(servo4,0,pulselength4);
      servo4.write(servostate4);
    }
    
    /*
    if (PS3.getButtonClick(UP)) {
      servostate4++;
      Serial.println(servostate4);
      servo4.write(servostate4);
    }
    if (PS3.getButtonClick(DOWN)) {
      servostate3++;
      Serial.println(servostate3);
      servo3.write(servostate3);
    }
    if (PS3.getButtonClick(RIGHT)) {
    }
    if (PS3.getButtonClick(LEFT)) {
    }*/
    
    if (PS3.getButtonPress(R1)){
      Serial.print(F("\r\nR1"));
      pixels.setPixelColor(0,pixels.Color(255,130,0));
      pixels.show();
    }
    else if (~PS3.getButtonPress(R1)){
      pixels.setPixelColor(0,pixels.Color(0,0,0));
      pixels.show();
    }
    if (PS3.getButtonPress(R2)){
      Serial.print(F("\r\nR2"));
      Laser(10);
    }
    else if (~PS3.getButtonPress(R2)){
      pixels.setPixelColor(0,pixels.Color(0,0,0));
      pixels.show();
    }
  }
}

void Laser(int SpeedDelay){
  float r,g,b;
  int red,green,blue;
  red=255,green=130,blue=0;
  
  for(int i = 0; i < 256;i=i+2){
    r = (i/256.0)*red;
    g = (i/256.0)*green;
    b = (i/256.0)*blue;
    setPixelColors(r,g,b,SpeedDelay);
  }
}

void setPixelColors(int red, int green, int blue, int SpeedDelay){
  setPixel(0,red,green,blue);
  pixels.show();
  delay(SpeedDelay);
}

void setPixel(int Pixel, byte red, byte green, byte blue) {
 #ifdef ADAFRUIT_NEOPIXEL_H
   // NeoPixel
   pixels.setPixelColor(Pixel, pixels.Color(red, green, blue));
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   leds[Pixel].r = red;
   leds[Pixel].g = green;
   leds[Pixel].b = blue;
 #endif
}

