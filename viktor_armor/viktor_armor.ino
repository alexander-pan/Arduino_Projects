// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            8
#define PIN2           9
#define PIN3           10
#define PINB           11
#define PIN4           12

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      24
#define NUMPIXELS2     9
#define NUMPIXELS3     9
#define NUMPIXELS4     35

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels_arm = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_chest = Adafruit_NeoPixel(NUMPIXELS2, PIN2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_shoulder = Adafruit_NeoPixel(NUMPIXELS3, PIN3, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_robot = Adafruit_NeoPixel(NUMPIXELS4, PIN4, NEO_GRB + NEO_KHZ800);

int delayval = 0; // delay for half a second
int phase = 0;
int phase_prime = 0;
int currentstate;

void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code

  //Initialize Neopixels
  pixels_arm.begin(); 
  pixels_chest.begin();
  pixels_shoulder.begin();
  pixels_robot.begin();

  //start the button state
  pinMode(PINB,INPUT_PULLUP);
  
}

void loop() {
  currentstate = digitalRead(PINB);

  if(currentstate == HIGH){
    FadeInOut(50);
  }
  if(currentstate == LOW){
    RunningLights(255,120,0,110);
  }
}

void setPixelColors(int red,int green, int blue){
  for(int i=0;i<NUMPIXELS4;i++){
    setPixelArm(i,red,green,blue);
    setPixelChest(i,red,green,blue);
    setPixelShoulder(i,red,green,blue);
    setPixelRobot(i,red,green,blue);
  }
  pixels_arm.show();
  pixels_chest.show();
  pixels_shoulder.show();
  pixels_robot.show();
  delay(5);
}

void setPixelRobot(int Pixel, byte red, byte green, byte blue) {
 #ifdef ADAFRUIT_NEOPIXEL_H
   // NeoPixel
   pixels_robot.setPixelColor(Pixel, pixels_robot.Color(red, green, blue));
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   leds[Pixel].r = red;
   leds[Pixel].g = green;
   leds[Pixel].b = blue;
 #endif
}

void setPixelArm(int Pixel, byte red, byte green, byte blue) {
 #ifdef ADAFRUIT_NEOPIXEL_H
   // NeoPixel
   pixels_arm.setPixelColor(Pixel, pixels_arm.Color(red, green, blue));
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   leds[Pixel].r = red;
   leds[Pixel].g = green;
   leds[Pixel].b = blue;
 #endif
}

void setPixelChest(int Pixel, byte red, byte green, byte blue) {
 #ifdef ADAFRUIT_NEOPIXEL_H
   // NeoPixel
   pixels_chest.setPixelColor(Pixel,pixels_chest.Color(red,green,blue));
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   leds[Pixel].r = red;
   leds[Pixel].g = green;
   leds[Pixel].b = blue;
 #endif
}

void setPixelShoulder(int Pixel, byte red, byte green, byte blue) {
 #ifdef ADAFRUIT_NEOPIXEL_H
   // NeoPixel
   pixels_shoulder.setPixelColor(Pixel,pixels_shoulder.Color(red,green,blue));  
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   leds[Pixel].r = red;
   leds[Pixel].g = green;
   leds[Pixel].b = blue;
 #endif
}

void FadeInOut(int SpeedDelay){
  float r,g,b;
  int red,green,blue;
  red=255,green=120,blue=0;
  
  for(int i = 255; i >= 0;i=i-2){
    r = (i/256.0)*red;
    g = (i/256.0)*green;
    b = (i/256.0)*blue;
    setPixelColors(r,g,b);
  }
  delay(SpeedDelay);
  for(int i = 0; i < 256;i=i+1){
    r = (i/256.0)*red;
    g = (i/256.0)*green;
    b = (i/256.0)*blue;
    setPixelColors(r,g,b);
  }
  delay(SpeedDelay/2);
}

void RunningLights(int red, int green, int blue, int SpeedDelay){
  int pos = 0;
  int alpha = 100; //alpha > 1
  for(int i=0;i < NUMPIXELS4*2;i++){
    pos--;
    for(int j=0;j < NUMPIXELS4;j++){
      phase = j+pos;
      setPixelArm(j,((sin(alpha*(phase))*127+128)/255)*red,
                 ((sin(alpha*(phase))*127+128)/255)*green,
                 ((sin(alpha*(phase))*127+128)/255)*blue);
      setPixelChest(j,((sin(alpha*(phase))*127+128)/255)*red,
                 ((sin(alpha*(phase))*127+128)/255)*green,
                 ((sin(alpha*(phase))*127+128)/255)*blue);
      setPixelShoulder(j,((sin(alpha*(phase))*127+128)/255)*red,
                 ((sin(alpha*(phase))*127+128)/255)*green,
                 ((sin(alpha*(phase))*127+128)/255)*blue);
      setPixelRobot(j,((sin(alpha*(phase))*127+128)/255)*red,
                 ((sin(alpha*(phase))*127+128)/255)*green,
                 ((sin(alpha*(phase))*127+128)/255)*blue);
    }  
    pixels_arm.show();
    pixels_chest.show();
    pixels_shoulder.show();
    pixels_robot.show();
    delay(SpeedDelay);
    //phase_prime = phase;
  }
}

