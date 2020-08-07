#include <avr/power.h>

//This Code operates the DC motor and controls it via potentiometer

int potPin = A0; //potentimotor PWM
int motorPin = 9; //motor pin 
int potValue = 0;
int motorValue = 0;

const int PINBUTTON = 8;//toggle on/off
int laststate = HIGH;
int currentstate;

void setup() {
 Serial.begin(9600);
 pinMode(PINBUTTON,INPUT_PULLUP);
 Serial.println("Switch is OFF");
 analogWrite(motorPin,0);
}
void loop() {
  currentstate = digitalRead(PINBUTTON);
  if(currentstate == HIGH && laststate==LOW){
    Serial.println("Switch is OFF");
    analogWrite(motorPin,0);
  }

  if(currentstate == LOW){
    Serial.println("Switch is ON");
    potValue = analogRead(potPin);  
    motorValue = map(potValue, 0, 1023, 0, 255);
    Serial.println(motorValue);
    analogWrite(motorPin, motorValue); 
    delay(2);
  }
  laststate = currentstate;
}
