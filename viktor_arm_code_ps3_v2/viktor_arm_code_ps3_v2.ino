/*
 Example sketch for the PS3 USB library - developed by Kristian Lauszus
 For more information visit my blog: http://blog.tkjelectronics.dk/ or
 send me an e-mail:  kristianl@tkjelectronics.com
 */

#include <PS3USB.h>
//#include <PS3BT.h>
//#include <usbhub.h>
#include <Wire.h>
#include <Servo.h> 
#include <Adafruit_PWMServoDriver.h>
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
//#define PINARM 5
#define PIXELS 1
//#define PIXELSARM 39

//neopixels
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(PIXELS, PIN, NEO_GRB + NEO_KHZ800);
//Adafruit_NeoPixel pixels_arm = Adafruit_NeoPixel(PIXELSARM, PINARM, NEO_GRB + NEO_KHZ800);

//Initialize all the USB Shield 
USB Usb;
/* You can create the instance of the class in two ways */
PS3USB PS3(&Usb); // This will just create the instance
//PS3USB PS3(&Usb,0x00,0x15,0x83,0x3D,0x0A,0x57); // This will also store the bluetooth address - this can be obtained from the dongle when running the sketch

//BTD Btd(&Usb);
//PS3BT PS3(&Btd);

//PWM Servo Driver
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
#define SERVOMIN  150 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // This is the 'maximum' pulse length count (out of 4096)
#define USMIN  600 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX  2400 // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

//Initiate numerical ints
int servo1,servo2,servo3,servo4,pulselength,pulselength2,pulselength3,pulselength4;
int servostate = 0;
int servostate2 = 0;
int servostate3 = 0;
int servostate4 = 0;

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

  //servo strtup
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates
  servo1 = 1;
  servo2 = 2;
  servo3 = 3;
  servo4 = 4;

  pulselength = map(servostate,0,180,SERVOMIN,SERVOMAX);
  pwm.setPWM(servo1,0,pulselength);
  pwm.setPWM(servo2,0,pulselength);
  pwm.setPWM(servo3,0,pulselength);
  pwm.setPWM(servo4,0,pulselength);

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
    //Activate Index Finger Individually
    if (PS3.getButtonPress(TRIANGLE)){
      Serial.println("Triangle");
      if(servostate > 100)
        servostate = 100;
      else
        servostate++;
      pulselength = map(servostate,0,180,SERVOMIN,SERVOMAX);
      Serial.println(pulselength);
      pwm.setPWM(servo1,0,pulselength);
    }
    else if (~PS3.getButtonPress(TRIANGLE) && servostate > 0){
      servostate = 0;
      pulselength = map(servostate,0,180,SERVOMIN,SERVOMAX);
      Serial.println(pulselength);
      pwm.setPWM(servo1,0,pulselength);
    }
    
    //Activate Middle Finger Individually
    if (PS3.getButtonPress(CIRCLE)){
      if(servostate2 > 130)
        servostate2 = 130;
      else
        servostate2++;
      pulselength2 = map(servostate2,0,180,SERVOMIN,SERVOMAX);
      pwm.setPWM(servo2,0,pulselength2);
    }
    else if (~PS3.getButtonPress(CIRCLE) && servostate2 > 0){
      servostate2 = 0;
      pulselength2 = map(servostate2,0,180,SERVOMIN,SERVOMAX);
      pwm.setPWM(servo2,0,pulselength2);
    }
    
    //Activate Ring Finger Individually
    if (PS3.getButtonPress(CROSS)){
      if(servostate3 > 100)
        servostate3 = 100;
      else
        servostate3++;
      pulselength3 = map(servostate3,0,180,SERVOMIN,SERVOMAX);
      pwm.setPWM(servo3,0,pulselength3);
    }
    else if (~PS3.getButtonPress(CROSS) && servostate3 > 0){
      servostate3 = -20;
      pulselength3 = map(servostate3,0,180,SERVOMIN,SERVOMAX);
      pwm.setPWM(servo3,0,pulselength3);
    }

    //Activate Thumb Finger Individually
    if (PS3.getButtonPress(SQUARE)){
      Serial.println("SQUARE");
      if(servostate4 > 100)
        servostate4 = 100;
      else
        servostate4++;
      pulselength4 = map(servostate4,0,180,SERVOMIN,SERVOMAX);
      pwm.setPWM(servo4,0,pulselength4);
    }
    else if (~PS3.getButtonPress(SQUARE) && servostate4 > 0){
      servostate4 = -20;
      pulselength4 = map(servostate4,0,180,SERVOMIN,SERVOMAX);
      pwm.setPWM(servo4,0,pulselength4);
    }
    
    /*
    if (PS3.getButtonClick(UP)) {
    }
    if (PS3.getButtonClick(DOWN)) {
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

