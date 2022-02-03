/*
 * rosserial PubSub Example
 * Prints "hello world!" and toggles led
 */
#include <Encoder.h>
#include <util/atomic.h>
#include <Servo.h>

// Pins
#define ENCA 2
#define ENCB 3
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

int pwr = 10;
int dir = 1;
int pos = 0;
float rpm;
int feedPwr=90;

int currTime = 0;

Servo mobileBase1, mobileBase2;
int a, encoder0PinALast=LOW;
int encoder0Pos = 0;
int time_H, time_L;
float deltaT;
float frequency=0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(ENCA, INPUT_PULLUP);
  pinMode(ENCB, INPUT_PULLUP);
  mobileBase1.attach(5);
  mobileBase2.attach(8);
  mobileBase1.write(98);
  mobileBase2.write(98);

  
}
void loop() {
   a = digitalRead(ENCA);
   currTime = millis();
//   Serial.println(a);
   if ((encoder0PinALast == LOW) && (a == HIGH)) {    // Signal A and B Direction  
     if (digitalRead(ENCB) == LOW) {
       encoder0Pos--;
       dir = -1;
//       Serial.print (" CW, ");       //  Direction CW
     } else {
       encoder0Pos++;
       dir = 1;       
//       Serial.print (" CCW, ");    //  Direction CCW       
     }
     
    //--------- Frequency Monitor ------------------   

     time_H = pulseIn(ENCA,HIGH);
     time_L = pulseIn(ENCA,LOW);     
     deltaT = time_H+time_L;
     deltaT = deltaT/1e6;
     frequency = 1.0/deltaT;
//     Serial.print(" F = ");
//     Serial.print(frequency); 
//     Serial.println(" Hz ");       
       
 
   } 
 encoder0PinALast = a;
 rpm = frequency/2000*60;
 Serial.println(rpm);
    // Low-pass filter (25 Hz cutoff)
//  rpmFilt = 0.854*rpmFilt + 0.0728*rpm + 0.0728*rpmPrev;
//  rpmPrev = rpm;
  ////////// PID control ////////////////////////
  float target_vel = 340;
//  Serial.println(target_vel);
  float kp = 1;
  float ki = 0;
//  float kd = 5;
  float new_pwr = computePID(kp,ki,target_vel, rpm, deltaT);
  ///////////////////////////////////////////////
  if (pwr < 90){
    dir = -1;
    }
  else{
    dir = 1;
    }
  pwr = (int) fabs(new_pwr);
  if (pwr > 110){
    pwr = 110;
    }
   else if (pwr < 70){
    pwr = 70;
   }
  run_motor(dir, pwr);
}
float computePID(float kp,float ki,float target_vel, float rpm, float deltaT){
  error = target_vel - rpm;
  cumuError += error*deltaT;
  float out_signal = kp*error + ki*cumuError;
  return out_signal;
  }
void run_motor(int dir, float pwr){
//  feedPwr = feedPwr+pwr;
  mobileBase1.write(pwr);
  mobileBase2.write(pwr);
//  if (dir == 1){
//    mobileBase1.write(90 + pwr);
//    mobileBase2.write(90 + pwr);
//  }
//  else if (dir == -1){
//    mobileBase1.write(90 - pwr);
//    mobileBase2.write(90 - pwr);
//  }
//  else{
//    mobileBase1.write(90);
//    mobileBase2.write(90);
//    }
 }
