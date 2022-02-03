/*
 * rosserial PubSub Example
 * Prints "hello world!" and toggles led
 */
#include <Encoder.h>
#include <util/atomic.h>
// Pins
#define ENCA 2
#define ENCB 3
#define PWM 10
#define IN1 5
#define IN2 6
long prevT = 0;
int posPrev = 0;
float cumuError = 0;
float error;
float lastError;
float rateError;
float rpmFilt = 0;
float rpmPrev = 0;
volatile int pos_i = 0;
volatile float velocity_i = 0;
volatile long prevT_i = 0;
volatile float deltaT_i;
int pwr = 255;
int dir = 1;
int pos = 0;
float rpm;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(ENCA, INPUT);
  pinMode(ENCB, INPUT);
  pinMode(PWM,OUTPUT);
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  attachInterrupt(digitalPinToInterrupt(ENCA),readEncoder,RISING);
}
void loop() {
  float deltaT;
  float velocity2 = 0;
  ////////// Calculate Speed (RPM)///////////////
//  run_motor(dir, pwr);
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
    velocity2 = velocity_i;
    deltaT = deltaT_i;
  }
  //Serial.println(velocity2);
  rpm = velocity2/20.0*60.0;
  Serial.println(rpm);
  ///////////////////////////////////////////////
    // Low-pass filter (25 Hz cutoff)
  rpmFilt = 0.854*rpmFilt + 0.0728*rpm + 0.0728*rpmPrev;
  rpmPrev = rpm;
  ////////// PID control ////////////////////////
  float target_vel = 120;
  float kp = 1;
  float ki = 1;
//  float kd = 5;
  float new_pwr = computePID(kp,ki,target_vel, rpmFilt, deltaT);
  ///////////////////////////////////////////////
  if (pwr < 0){
    dir = -1;
    }
  else{
    dir = 1;
    }
  pwr = (int) fabs(new_pwr);
  if (pwr > 255){
    pwr = 255;
    }
  run_motor(dir, pwr);
}
float computePID(float kp,float ki,float target_vel, float rpm, float deltaT){
  error = target_vel - rpmFilt;
  cumuError += error*deltaT;
  float out_signal = kp*error + ki*cumuError;
  return out_signal;
  }
void run_motor(int dir, float pwr){
  if (dir == 1){
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
  }
  else if (dir == -1){
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
  }
  else{
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    }
  analogWrite(PWM, pwr);
  }
void readEncoder(){
  // Read encoder B when ENCA rises
  int b = digitalRead(ENCB);
  int increment = 1;
  // Compute velocity with method 2
  long currT = micros();
  deltaT_i = ((float) (currT - prevT_i))/1.0e6;
  velocity_i = increment/deltaT_i;
  prevT_i = currT;
}
