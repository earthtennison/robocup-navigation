//joystick
#include <Servo.h>
#include <Encoder.h>


Servo mobileBase1,mobileBase2;
Encoder enc1(2,3);
int x = A0;
int y = A1;
int sw = 2;
//int led = 13;
long oldX = -999;
unsigned long oldTime = 0;
uint16_t oldReadVX = 1790;
uint16_t refVX;
uint16_t feedVX;
void setup() {
  Serial.begin(57600);
  pinMode(sw, INPUT_PULLUP);
  mobileBase1.attach(9);
  mobileBase2.attach(10);
}
void loop() {
  uint16_t newReadVX = analogRead(x); // Max Speed = 3580 RPM
  if (abs(newReadVX-oldReadVX)>5){
    refVX = newReadVX;
    oldReadVX = newReadVX;
//    Serial.print("Reference X: "); Serial.println(refVX);
//    Serial.print("Input X: "); Serial.println(newReadVX);
  }
  long newX = enc1.read();
  unsigned long newTime = millis();
  if (newX != oldX){
    feedVX = map(abs(newX-oldX),0,1000,0,90)*500/3/(newTime-oldTime);
    int8_t sign;
    if (newX-oldX<0) sign = -1;
    else sign = 1;
//    Serial.print("Difference: "); Serial.println(map(newX-oldX,-200,200,-90,90));
//    Serial.print("Interval: "); Serial.println(newTime-oldTime);
    oldTime = newTime;
    oldX = newX;
    Serial.print("Speed: "); Serial.println(sign*feedVX);
  }
//  int btn = digitalRead(sw);
//  if (sign*feedVX < map(refVX,0,1023,-3580,3580)){
//    mobileBase1.write(map(++refVX,-3580,3580,10,170));
//    mobileBase2.write(map(++refVX,-3580,3580,10,170));
//  }
}
