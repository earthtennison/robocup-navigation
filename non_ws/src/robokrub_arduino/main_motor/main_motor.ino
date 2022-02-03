#include <Servo.h>
Servo ServoA;
Servo ServoB;
Servo ServoC;
Servo ServoD;
int count = 95;
void setup() {
  // put your setup code here, to run once:
//  ServoA.attach(4);
  ServoB.attach(5);
//  ServoC.attach(6);
  ServoD.attach(8);
  Serial.begin(9600);
}
void loop() {
   //put your main code here, to run repeatedly:
//  ServoA.write(100);
//  forward test
  while(count <= 170){
    ServoB.write(count);
    ServoD.write(count);
    Serial.println(count);
    count += 5;
    delay(60000);
    ServoB.write(90);
    ServoD.write(90);
    delay(30000);
    }
  //stop test
  count = 90;
  ServoB.write(count);
  ServoD.write(count);
  Serial.println(count);
  delay(10000);
  //backward test
  while(count >= 10){
    ServoB.write(count);
    ServoD.write(count);
    Serial.println(count);
    count -= 5;
    delay(60000);
    ServoB.write(90);
    ServoD.write(90);
    delay(30000);
    }
  //stop test
  count = 90;
  ServoB.write(count);
  ServoD.write(count);
  Serial.println(count);
  delay(10000);
}
