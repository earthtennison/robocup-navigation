#include <Servo.h>
#include <util/atomic.h>

#define ENCA 2
#define ENCB 3

Servo ServoB;
Servo ServoD;

int count = 90;
float rpm;

volatile float velocity_i = 0;
volatile long prevT_i = 0;
volatile float deltaT_i = 0;

void setup() {
  // put your setup code here, to run once:
  ServoB.attach(5);
  ServoD.attach(8);
  Serial.begin(115200);
  pinMode(ENCA, INPUT_PULLUP);
  pinMode(ENCB, INPUT_PULLUP);


  attachInterrupt(digitalPinToInterrupt(ENCA),readEncoder,RISING);
}
void loop() {
  // put your main code here, to run repeatedly:
//  ServoA.write(100);
  //forward test
//  ServoB.write(95);
//  ServoD.write(95);
  float deltaT = 0;
  float velocity2 = 0;
  ////////// Calculate Speed (RPM)///////////////
//  run_motor(dir, pwr);
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
    velocity2 = velocity_i;
    deltaT = deltaT_i;
  }

//  rpm = velocity2/2000.0*60.0;
  Serial.println(velocity2);
}
void readEncoder(){
  // Read encoder B when ENCA rises
  int b = digitalRead(ENCB);
  int increment = 0;
  if(b>0){
    // If B is high, increment forward
    increment = 1;
  }
  else{
    // Otherwise, increment backward
    increment = -1;
  }
  // Compute velocity with method 2
  long currT = micros();
  deltaT_i = ((float) (currT - prevT_i))/1.0e6;
  velocity_i = increment/deltaT_i;
  prevT_i = currT;
}
