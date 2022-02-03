#include <util/atomic.h>

// Pins
#define ENCA 12
#define PWM 10
#define IN1 5
#define IN2 6

// globals
long prevT = 0;
int posPrev = 0;
// Use the "volatile" directive for variables
// used in an interrupt
volatile int pos_i = 0;
volatile float velocity_i = 0;
volatile long prevT_i = 0;
float newTime;
float oldTime=0;
int detectState;
int count;
boolean up=false;
float vel=0;
float prevTime = 0;

float v1Filt = 0;
float v1Prev = 0;
float v2Filt = 0;
float v2Prev = 0;

float eintegral = 0;

void setup() {
  Serial.begin(115200);

  pinMode(ENCA,INPUT);

  pinMode(PWM,OUTPUT);
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);

}

void loop() {

  newTime=millis();
  detectState = digitalRead(12);
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
//    pos = pos_i;
//    velocity2 = velocity_i;
  }
  if(detectState==1){
    up=true;
  }
  else{
    if(up){
      count++;
    }
    up=false;
  }
  if(newTime-oldTime>200){
    vel = (count/20)*1000*60/(newTime-oldTime);
    oldTime=newTime;
    count=0;
//    Serial.println(vel);
  }
  digitalWrite(5,HIGH);
  digitalWrite(6,LOW);
  analogWrite(10,255);

  // Set a target
  float vt = 300;
  v1Filt = vel;

  float deltaT = newTime-prevTime;
  
  // Compute the control signal u
  float kp = 5;
  float ki = 10;
  float e = vt-v1Filt;
  eintegral = eintegral + e*deltaT;
  
  float u = kp*e + ki*eintegral;

  // Set the motor speed and direction
  int dir = 1;
  if (u<0){
    dir = -1;
  }
  int pwr = (int) fabs(u);
  if(pwr > 255){
    pwr = 255;
  }
  prevTime = newTime;
  setMotor(dir,pwr,PWM,IN1,IN2);

  Serial.print(pwr);
  Serial.print(" ");
  Serial.print(v1Filt);
  Serial.println();
  delay(1);
}

void setMotor(int dir, int pwmVal, int pwm, int in1, int in2){
  analogWrite(pwm,pwmVal); // Motor speed
  if(dir == 1){ 
    // Turn one way
    digitalWrite(in1,HIGH);
    digitalWrite(in2,LOW);
  }
  else if(dir == -1){
    // Turn the other way
    digitalWrite(in1,LOW);
    digitalWrite(in2,HIGH);
  }
  else{
    // Or dont turn
    digitalWrite(in1,LOW);
    digitalWrite(in2,LOW);    
  }
}
