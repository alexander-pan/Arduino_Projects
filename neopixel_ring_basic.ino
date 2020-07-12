#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 6
#define NUMPIXELS 24
#define BRIGHTNESS 255
#define DELAY 100
// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup()
{
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  // End of trinket special code
  
  pixels.setBrightness(BRIGHTNESS); //initialize brightness
  pixels.begin(); // This initializes the NeoPixel library.
  pixels.show();
}
 
void loop(){
  //FadeInOut(60,0,20,255,50,0);
  //delay(5000);
  //meteorRain(60,0,20,20, 64, true, 50);
  Fire(10,120,100);
  //delay(500);

}

void setColors(int r1, int g1, int b1, int r2, int g2, int b2){
  for(int i=0;i<NUMPIXELS;i++){
    pixels.setPixelColor(i,r1,g1,b1);
  }
  /*for(int i=5;i<8;i++){
    pixels.setPixelColor(i,r2,g2,b2);
  }
  for(int i=11;i<14;i++){
    pixels.setPixelColor(i,r2,g2,b2);
  }
  for(int i=17;i<20;i++){
    pixels.setPixelColor(i,r2,g2,b2);
  }
  for(int i=23;i<NUMPIXELS;i++){
    pixels.setPixelColor(i,r2,g2,b2);
  }
  for(int i=0;i<2;i++){
    pixels.setPixelColor(i,r2,g2,b2);
  }*/
  pixels.show();
  delay(10);
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

void setAll(int r1, int g1, int b1){
  for(int i=0;i < pixels.numPixels(); i++){
      pixels.setPixelColor(i,r1,g1,b1);
    }
}

void FadeInOut(int red, int green, int blue, int red2, int green2, int blue2){
  float r1,r2,g1,g2,b1,b2;

  for(int i = 255; i >= 0;i=i-2){
    r1 = (i/256.0)*red;
    g1 = (i/256.0)*green;
    b1 = (i/256.0)*blue;

    r2 = (i/256.0)*red2;
    g2 = (i/256.0)*green2;
    b2 = (i/256.0)*blue2;
    setColors(r1,g1,b1,r2,g2,b2);
  }
  for(int i = 0; i < 256;i=i+2){
    r1 = (i/256.0)*red;
    g1 = (i/256.0)*green;
    b1 = (i/256.0)*blue;

    r2 = (i/256.0)*red2;
    g2 = (i/256.0)*green2;
    b2 = (i/256.0)*blue2;
    setColors(r1,g1,b1,r2,g2,b2);
  }
}

void meteorRain(int red, int green, int blue, byte meteorSize, byte meteorTrailDecay, boolean meteorRandomDecay, int SpeedDelay) {  
  setAll(0,0,0);
  int NUM_LEDS = pixels.numPixels();
  for(int i = 0; i < NUM_LEDS+NUM_LEDS; i++) {
    
    
//     fade brightness all LEDs one step
    for(int j=0; j<NUM_LEDS; j++) {
      if( (!meteorRandomDecay) || (random(10)>5) ) {
        fadeToBlack(j, meteorTrailDecay );        
      }
    }
    
    // draw meteor
    for(int j = 0; j < meteorSize; j++) {
      if( ( i-j <NUM_LEDS) && (i-j>=0) ) {
        pixels.setPixelColor(i-j, red, green, blue);
      } 
    }
   
    pixels.show();
    delay(SpeedDelay);
  }
}

void fadeToBlack(int ledNo, byte fadeValue) {
 #ifdef ADAFRUIT_NEOPIXEL_H 
    // NeoPixel
    uint32_t oldColor;
    uint8_t r, g, b;
    int value;
    
    oldColor = pixels.getPixelColor(ledNo);
    r = (oldColor & 0x00ff0000UL) >> 16;
    g = (oldColor & 0x0000ff00UL) >> 8;
    b = (oldColor & 0x000000ffUL);

    r=(r<=10)? 0 : (int) r-(r*fadeValue/256);
    g=(g<=10)? 0 : (int) g-(g*fadeValue/256);
    b=(b<=10)? 0 : (int) b-(b*fadeValue/256);
    
    pixels.setPixelColor(ledNo, r,g,b);
 #endif
}

void Fire(int Cooling, int Sparking, int SpeedDelay) {
  int NUM_LEDS = pixels.numPixels();
  int heat[NUM_LEDS];
  int cooldown;
 
  // Step 1.  Cool down every cell a little
  for( int i = 0; i < NUM_LEDS; i++) {
    cooldown = random(0, ((Cooling * 10) / NUM_LEDS) + 2);
   
    if(cooldown>heat[i]) {
      heat[i]=0;
    } else {
      heat[i]=heat[i]-cooldown;
    }
  }
 
  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for( int k= NUM_LEDS - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
  }
   
  // Step 3.  Randomly ignite new 'sparks' near the bottom
  if( random(255) < Sparking ) {
    int y = random(7);
    heat[y] = heat[y] + random(160,255);
    //heat[y] = random(160,255);
  }

  // Step 4.  Convert heat to LED colors
  for( int j = 0; j < NUM_LEDS; j++) {
    setPixelHeatColor(j, heat[j] );
  }

  pixels.show();
  delay(SpeedDelay);
}

void setPixelHeatColor (int Pixel, byte temperature) {
  // Scale 'heat' down from 0-255 to 0-191
  byte t192 = round((temperature/255.0)*191);
 
  // calculate ramp up from
  byte heatramp = t192 & 0x3F; // 0..63
  heatramp <<= 2; // scale up to 0..252
 
  // figure out which third of the spectrum we're in:
  if( t192 > 0x80) {                     // hottest
    setPixel(Pixel, 255,0, heatramp);
  } else if( t192 > 0x40 ) {             // middle
    setPixel(Pixel, 255, heatramp, 0);
  } else {                               // coolest
    setPixel(Pixel, 0, 0, 0);
  }
}
