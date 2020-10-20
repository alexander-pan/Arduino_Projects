#include <avr/power.h>

//This Code operates the DC motor and controls it via potentiometer

int potPin = 3; //potentimotor
int motorPin = 4; //motor pin
int potValue = 0;
int motorValue = 0;

const int PINBUTTON1 = 1;//toggle on/off
int laststate = LOW;
int currentstate;

void setup() {
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
 //Serial.begin(9600);
 //pinMode(PINBUTTON1,INPUT_PULLUP);
 //Serial.println("Switch is OFF");
 //analogWrite(motorPin,0);
}
void loop() {
  /**
  //currentstate = digitalRead(PINBUTTON1);
  if(currentstate == LOW){
    //Serial.println("Switch is OFF");
    analogWrite(motorPin,0);
  }

  if(currentstate == HIGH){
    analogWrite(motorPin,255);
    //Serial.println("Switch is ON");
    //potValue = analogRead(potPin);  
    //motorValue = map(potValue, 0, 1023, 0, 255);
    //analogWrite(motorPin, motorValue);  
    //delay(2);
  }
  laststate = currentstate;*/
  //analogWrite(motorPin,50);
  digitalWrite(motorPin,LOW);
}
