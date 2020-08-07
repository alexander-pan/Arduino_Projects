#include <Adafruit_NeoPixel.h>      //Include AdaFruit NEO Library
#define PINLR            6 //NEO RING 1
#define PINSR            5 //NEO RING 2
#define NUMPIXELS_LR     16
#define NUMPIXELS_SR     12 
#define BRIGHTNESS   255
#define DELAY          100

const int PINBUTTON1 = 8;
const int PINBUTTON2 = 9;
const int PINBUTTON3 = 10;
const int PINBUTTON4 = 11;
const int PINBUTTON5 = 12;
const int PINBUTTON6 = 3;

Adafruit_NeoPixel pixels_lr = Adafruit_NeoPixel(NUMPIXELS_LR, PINLR, NEO_GRB + NEO_KHZ800);     //AdaFruit Neo Library
Adafruit_NeoPixel pixels_sr = Adafruit_NeoPixel(NUMPIXELS_SR, PINSR, NEO_GRB + NEO_KHZ800);     //AdaFruit Neo Library

//button's states
int laststate[6] = {LOW,LOW,LOW,LOW,LOW,LOW};
int currentstate[6];

//global variables to  be used throughout
int hex_color = 1; //1: Red 2: Blue 3: Yellow 4:Purple
int effect = 1; //1:Solid Color 2: Sparkle
int p[4],s[4];//keep the previous windmill positions 

//Initializations
void setup() {
  //the pull-up input pin will be HIGH when the switch isopen and Low when the switch is closed
  pinMode(PINBUTTON1,INPUT_PULLUP);
  pinMode(PINBUTTON2,INPUT_PULLUP);
  pinMode(PINBUTTON3,INPUT_PULLUP);
  pinMode(PINBUTTON4,INPUT_PULLUP);
  pinMode(PINBUTTON5,INPUT_PULLUP);
  pinMode(PINBUTTON6,INPUT_PULLUP);
  
  //initialize pixels in ring
  pixels_lr.setBrightness(BRIGHTNESS);
  pixels_sr.setBrightness(BRIGHTNESS);
  pixels_lr.begin();
  pixels_sr.begin();

  //Sets Initial Color for Hex Color
  changeHexColor(1);
  //initalize serial communication at 9600 bits per second
  Serial.begin(9600);
}

//Functions to Loop through
void loop() {
  currentstate[0] = digitalRead(PINBUTTON1);
  currentstate[1] = digitalRead(PINBUTTON2);
  currentstate[2] = digitalRead(PINBUTTON3);
  currentstate[3] = digitalRead(PINBUTTON4);
  currentstate[4] = digitalRead(PINBUTTON5);  
  currentstate[5] = digitalRead(PINBUTTON6);
  
  //BUTTON 1: Changes to Blue Default Color
  if(laststate[0] == HIGH && currentstate[0] == LOW && currentstate[1] == LOW && currentstate[2] == LOW){
    Serial.println("The state is from LOW to HIGH for B1");
    //Serial.println(currentstate[0]);
    hex_color = 1;
  }
  if(currentstate[0] ==HIGH){
    Serial.println("the state is from High to LOW for B1");
    //Serial.println(currentstate[0]);
    hex_color = 2;
  }
  laststate[0] = currentstate[0];

  //BUTTON 2
  if(laststate[1] == HIGH && currentstate[1] == LOW && currentstate[0] == LOW && currentstate[2] == LOW){
    Serial.println("The state is from LOW to HIGH for B2");
    hex_color = 1;
  }
  if(currentstate[1] ==HIGH){
    Serial.println("the state is from High to LOW for B2");
   hex_color = 3;
  }
  laststate[1] = currentstate[1];

  //BUTTON 3
  if(laststate[2] == HIGH && currentstate[2] == LOW && currentstate[0] == LOW && currentstate[1] == LOW){
    Serial.println("The state is from LOW to HIGH for B3");
    hex_color = 1;
  }
  if(currentstate[2] ==HIGH){
    Serial.println("the state is from High to LOW for B3");
    hex_color = 4;
  }
  laststate[2] = currentstate[2];

  //BUTTON 4
  if(laststate[3] == HIGH && currentstate[3] == LOW){
    Serial.println("The state is from LOW to HIGH for B4");
    effect = 1;
  }
  if(currentstate[3] ==HIGH){
    Serial.println("the state is from High to LOW for B4");
    effect = 3;
  }
  laststate[3] = currentstate[3];

  //BUTTON 5
  if(laststate[4] == HIGH && currentstate[4] == LOW){
    Serial.println("The state is from LOW to HIGH for B4");
    effect = 1;
  }
  if(currentstate[4] ==HIGH){
    Serial.println("the state is from High to LOW for B4");
    effect = 4;
  }
  laststate[4] = currentstate[4];

  //BUTTON 6
  if(laststate[5] == HIGH && currentstate[5] == LOW){
    Serial.println("The state is from LOW to HIGH for B4");
    effect = 1;
  }
  if(currentstate[5] ==HIGH){
    Serial.println("the state is from High to LOW for B4");
    effect = 5;
  }
  laststate[5] = currentstate[5];
  
  //Runs Current Active Effect
  changeFXEffect(effect,hex_color);
}


/*************************************************
 ************************************************* 
 *************************************************
 *************************************************/
void changeHexColor(int HexColor){
  switch(HexColor){
      case 1://Red
        setRingColors(255,0,0);
        break;
      case 2://Blue
        setRingColors(0,0,255);
        break;
      case 3://Yellow
        setRingColors(255,150,0);
        break;
      case 4://Purple
        setRingColors(180,0,255);
        break;
    }
}

void changeFXEffect(int FX,int HexColor){
  switch(FX){
    case 1://Solid Color
      changeHexColor(HexColor);
      break;
    case 2://Sparkle/Heat Temperature Change
      Fire(10,120,100,HexColor);
      break;
    case 3://Fade
      FadeInOut(0,HexColor);
      break;
    case 4://Sparkle
      Sparkle(30,HexColor,2);
      break;
    case 5://Windmill
      Windmill(50,HexColor);
      break;
  }
}
void setRingColors(int red,int green, int blue){
  for(int i=0;i<NUMPIXELS_LR;i++){
    setPixel_LR(i,red,green,blue);
  }
  for(int i=0;i<NUMPIXELS_SR;i++){
    setPixel_SR(i,red,green,blue);
  }
  pixels_lr.show();
  pixels_sr.show();
  delay(10);
}

void setPixel_LR(int Pixel, byte red, byte green, byte blue) {
 #ifdef ADAFRUIT_NEOPIXEL_H
   // NeoPixel
   pixels_lr.setPixelColor(Pixel, pixels_lr.Color(red, green, blue));
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   leds[Pixel].r = red;
   leds[Pixel].g = green;
   leds[Pixel].b = blue;
 #endif
}
void setPixel_SR(int Pixel, byte red, byte green, byte blue) {
 #ifdef ADAFRUIT_NEOPIXEL_H
   // NeoPixel
   pixels_sr.setPixelColor(Pixel, pixels_sr.Color(red, green, blue));
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   leds[Pixel].r = red;
   leds[Pixel].g = green;
   leds[Pixel].b = blue;
 #endif
}

void Fire(int Cooling, int Sparking, int SpeedDelay, int HexColor) {
  int NUM_LEDS = pixels_lr.numPixels();
  int NUM_LEDS2 = pixels_sr.numPixels();
  int heat[NUM_LEDS], heat2[NUM_LEDS];
  int cooldown,cooldown2;
 
  // Step 1.  Cool down every cell a little
  for( int i = 0; i < NUM_LEDS; i++) {
    cooldown = random(0, ((Cooling * 10) / NUM_LEDS) + 2);
   
    if(cooldown>heat[i]) {
      heat[i]=0;
    } else {
      heat[i]=heat[i]-cooldown;
    }
  }

  for( int i = 0; i < NUM_LEDS2; i++) {
    cooldown2 = random(0, ((Cooling * 10) / NUM_LEDS2) + 2);
   
    if(cooldown2>heat2[i]) {
      heat2[i]=0;
    } else {
      heat2[i]=heat2[i]-cooldown2;
    }
  }
 
  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for( int k= NUM_LEDS - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
  }

  for( int k= NUM_LEDS2 - 1; k >= 2; k--) {
    heat2[k] = (heat2[k - 1] + heat2[k - 2] + heat2[k - 2]) / 3;
  }
   
  // Step 3.  Randomly ignite new 'sparks' near the bottom
  if( random(255) < Sparking ) {
    int y = random(7);
    //heat[y] = heat[y] + random(160,255);
    heat[y] = random(160,255);
  }

  if( random(255) < Sparking ) {
    int y = random(7);
    //heat2[y] = heat2[y] + random(160,255);
    heat2[y] = random(160,255);
  }

  // Step 4.  Convert heat to LED colors
  for( int j = 0; j < NUM_LEDS; j++) {
    setPixelHeatColor(j, heat[j],HexColor);
  }

  for( int j = 0; j < NUM_LEDS2; j++) {
    setPixelHeatColor(j, heat2[j],HexColor);
  }

  pixels_lr.show();
  pixels_sr.show();
  delay(SpeedDelay);
}

void setPixelHeatColor (int Pixel, int temperature,int HexColor) {
  // Scale 'heat' down from 0-255 to 0-191
  int t192 = temperature;//round((temperature/255.0)*191);
 
  // calculate ramp up from
  byte heatramp = t192 & 0x3F; // 0..63
  heatramp <<= 2; // scale up to 0..252
  
  // figure out which third of the spectrum we're in:
  if(HexColor == 1){
    if( t192 > 0x80) {                     // hottest
      setPixel_LR(Pixel, heatramp,0, 0);
      setPixel_SR(Pixel, heatramp,0, 0);
    } else if( t192 > 0x40 ) {             // middle
      setPixel_LR(Pixel, heatramp,0, 0);
      setPixel_SR(Pixel, heatramp,0, 0);
    } else {                               // coolest
      setPixel_LR(Pixel, 0, 0, 0);
      setPixel_SR(Pixel, 0, 0, 0);
    }
  } else if(HexColor == 2){
    if( t192 > 0x80) {                     // hottest
      setPixel_LR(Pixel, 0,0, heatramp);
      setPixel_SR(Pixel, 0,0, heatramp);
    } else if( t192 > 0x40 ) {             // middle
      setPixel_LR(Pixel, 0,0, heatramp);
      setPixel_SR(Pixel, 0,0, heatramp);
    } else {                               // coolest
      setPixel_LR(Pixel, 0, 0, 0);
      setPixel_SR(Pixel, 0, 0, 0);
    }
  } else if(HexColor == 3){
    if( t192 > 0x80) {                     // hottest
      if(heatramp < 200)
        heatramp = 255;
      setPixel_LR(Pixel, heatramp,150, 0);
      setPixel_SR(Pixel, heatramp,150, 0);
    } else if( t192 > 0x40 ) {             // middle
      if(heatramp < 100)
        heatramp = 200;
      setPixel_LR(Pixel, heatramp,80, 0);
      setPixel_SR(Pixel, heatramp,80, 0);
    } else {                               // coolest
      setPixel_LR(Pixel, 0, 0, 0);
      setPixel_SR(Pixel, 0, 0, 0);
    }
  } else if(HexColor == 4){
    if( t192 > 0x80) {                     // hottest
      if(heatramp < 200)
        heatramp = 255;
      setPixel_LR(Pixel, 180,0, heatramp);
      setPixel_SR(Pixel, 180,0, heatramp);
    } else if( t192 > 0x40 ) {             // middle
      if(heatramp < 100)
        heatramp = 200;
      setPixel_LR(Pixel, 80,0, heatramp);
      setPixel_SR(Pixel, 80,0, heatramp);
    } else {                               // coolest
      setPixel_LR(Pixel, 0, 0, 0);
      setPixel_SR(Pixel, 0, 0, 0);
    }
  }
}

void FadeInOut(int SpeedDelay, int HexColor){
  float r,g,b;
  int red,green,blue;
  switch(HexColor){
    case 1:
      red = 255, green = 0, blue = 0;
      break;
    case 2:
      red = 0, green = 0, blue = 255;
      break;
    case 3:
      red = 255, green = 150, blue = 0;
      break;
    case 4:
      red = 180, green = 0, blue = 255;
      break;
  }
  
  for(int i = 255; i >= 0;i=i-2){
    r = (i/256.0)*red;
    g = (i/256.0)*green;
    b = (i/256.0)*blue;
    setRingColors(r,g,b);
  }
  //delay(SpeedDelay);
  for(int i = 0; i < 256;i=i+1){
    r = (i/256.0)*red;
    g = (i/256.0)*green;
    b = (i/256.0)*blue;
    setRingColors(r,g,b);
  }
  //delay(SpeedDelay/2);
}

void Sparkle(int SpeedDelay,int HexColor, int N){
  int red,green,blue;
  int lr[N],sr[N];
  switch(HexColor){
    case 1:
      red = 255, green = 0, blue = 0;
      break;
    case 2:
      red = 0, green = 0, blue = 255;
      break;
    case 3:
      red = 255, green = 150, blue = 0;
      break;
    case 4:
      red = 180, green = 0, blue = 255;
      break;
  }
  for(int i = 0;i < N;i++){
    lr[i] = random(NUMPIXELS_LR);
    sr[i] = random(NUMPIXELS_SR);
    setPixel_LR(lr[i],red,green,blue);
    setPixel_SR(sr[i],red,green,blue);
  }
  delay(SpeedDelay);
  
  pixels_lr.show();
  pixels_sr.show();
  
  delay(SpeedDelay);

  for(int i = 0;i < N;i++){
    setPixel_LR(lr[i],0,0,0);
    setPixel_SR(sr[i],0,0,0);
  }
}
int getWindmillPos(int i, int NumPixels, int p){
  switch(p){
    case 1:
      if(i+NumPixels/4 >= NumPixels)
        return i - NumPixels*3/4;
      else
        return i + NumPixels/4;
    case 2:
      if(i+NumPixels/2 >= NumPixels)
        return i - NumPixels/2;
      else
        return i + NumPixels/2;
    case 3:
      if(i+NumPixels*3/4 >= NumPixels)
        return i - NumPixels/4;
      else
        return i + NumPixels*3/4;
    case 0:
      return i;
  }   
}
void Windmill(int SpeedDelay, int HexColor){
  int red,green,blue,k;
  
  switch(HexColor){
    case 1:
      red = 255, green = 0, blue = 0;
      break;
    case 2:
      red = 0, green = 0, blue = 255;
      break;
    case 3:
      red = 255, green = 150, blue = 0;
      break;
    case 4:
      red = 180, green = 0, blue = 255;
      break;
  }
  //Run the Windmill of Lights
  for(int i = 0;i < NUMPIXELS_LR;i++){
    if(i >= NUMPIXELS_SR)
      k = i-NUMPIXELS_SR;
    else
      k = i;
      
    for(int j = 0;j < 4;j++){
      p[j] = getWindmillPos(i,NUMPIXELS_LR,j);
      s[j] = getWindmillPos(k,NUMPIXELS_SR,j);
      setPixel_LR(p[j],red,green,blue);
      setPixel_SR(s[j],red,green,blue);
    }
    pixels_lr.show();
    pixels_sr.show();
    delay(SpeedDelay);

    for(int j = 0;j < 4;j++){
      setPixel_LR(p[j],0,0,0);
    }
    for(int j = 0;j < 4;j++){
      setPixel_SR(s[j],0,0,0);
    }
    pixels_lr.show();
    pixels_sr.show();
  } 
}
