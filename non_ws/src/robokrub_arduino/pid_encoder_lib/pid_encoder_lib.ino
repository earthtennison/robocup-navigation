
#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Servo.h>
#include <Encoder.h>
#include <math.h>
Servo myservo1, myservo2;
Encoder enc(18,19),enc2(2,3);
long pos = 0;
long posPrev = 0;
long pos2 = 0;
long posPrev2 = 0;
unsigned long currT = 0;
unsigned long prevT = 0;
float velocity;
float velocity2;

#define POWER 100

void setup()
{
  Serial.begin(9600);
  myservo1.attach(5);
  myservo2.attach(8);

//  myservo1.write(POWER);
  myservo2.write(POWER);
//  myservo1.write(POWER);
//  myservo2.write(POWER);

}
void loop()
{
  currT = millis();
  pos = enc.read();
  pos2 = enc2.read();
  float deltaT = (float) (currT-prevT)/1e3;
  Serial.println(pos);
  velocity = ((pos-posPrev)/deltaT)/1600.0*60;
  velocity2 = ((posPrev2-pos2)/deltaT)/1600.0*60;
//  Serial.println(velocity2);
  posPrev2 = pos2;
  posPrev = pos;
  prevT = currT;


//  myservo1.write(POWER);
//  myservo2.write(90);
//  myservo1.write(POWER);
//  delay(5000);
//  myservo2.write(90);
//  myservo1.write(90);
//  delay(10000);
}
