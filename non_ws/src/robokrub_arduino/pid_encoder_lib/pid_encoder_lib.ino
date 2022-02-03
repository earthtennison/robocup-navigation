
#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Servo.h>
#include <Encoder.h>
#include <math.h>
Servo myservo1, myservo2;
Encoder enc(18,19);
long pos = 0;
long posPrev = 0;
unsigned long currT = 0;
unsigned long prevT = 0;
float velocity;

#define POWER 100

void setup()
{
  Serial.begin(9600);
  myservo1.attach(5);
  myservo2.attach(8);

  myservo2.write(POWER);
//  myservo1.write(POWER);
//  myservo2.write(POWER);

}
void loop()
{
  currT = millis();
  pos = enc.read();
  float deltaT = (float) (currT-prevT)/1e3;
  Serial.println(pos);
  velocity = ((pos-posPrev)/deltaT)/1600.0*60;
//  Serial.println(velocity);
  posPrev = pos;
  prevT = currT;


  myservo2.write(POWER);
//  myservo1.write(POWER);
  delay(5000);
  myservo2.write(90);
//  myservo1.write(90);
  delay(10000);
}
