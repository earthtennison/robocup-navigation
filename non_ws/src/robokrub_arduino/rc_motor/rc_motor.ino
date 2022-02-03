#include <util/atomic.h>
#include <ros.h>
#include <std_msgs/Float32.h>
#include <geometry_msgs/Twist.h>

// Pins
#define ENCA 3
#define ENCB 2
#define PWM 10
#define IN1 5
#define IN2 6
#define WHEEL_RAD 0.125
#define WHEEL_DIST 0.5
#define PUB_PERIOD 1000


std_msgs::Float32 goal_left_msg;
std_msgs::Float32 current_vel_msg;
std_msgs::Float32 pwr_msg;

ros::Publisher goal_left_pub("goal_left", &goal_left_msg);
ros::Publisher current_vel_pub("current_vel", &current_vel_msg);
ros::Publisher pwr_pub("pwr", &pwr_msg);


ros::NodeHandle  nh;

float goal_left=0;
unsigned long newTime = 0;
unsigned long oldTime = 0;

float velFilt=0;
float velPrev=0;
long prevT = 0;
int posPrev = 0;
float cumuError = 0;
float error;
float diffError;
volatile int pos_i = 0;
volatile float velocity_i = 0;
volatile long prevT_i = 0;
volatile float deltaT_i;
int pwr = 100;
int dir = 1;
int pos = 0;
float vel;
float last_error = 0;

void TwistCb( const geometry_msgs::Twist& twist_msg){

  float x = twist_msg.linear.x;
  float z = twist_msg.angular.z;

  goal_left = x-z*WHEEL_DIST/2;
  
}

ros::Subscriber<geometry_msgs::Twist> sub("cmd_vel", TwistCb );

void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  nh.getHardware()->setBaud(57600);
  nh.initNode();

  nh.advertise(goal_left_pub);
  nh.advertise(current_vel_pub);
  nh.advertise(pwr_pub);
  nh.subscribe(sub);
  
  pinMode(ENCA, INPUT);
  pinMode(PWM,OUTPUT);
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  attachInterrupt(digitalPinToInterrupt(ENCA),readEncoder,RISING);
}
void loop() {
  nh.spinOnce();
  
  newTime = millis();
  float deltaT;
  float velocity2 = 0;
  ////////// Calculate Speed (RPM)///////////////
//  run_motor(dir, pwr);
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
    velocity2 = velocity_i;
    deltaT = deltaT_i;
  }
  //Serial.println(velocity2);
  vel = (velocity2/20.0)*2*M_PI*WHEEL_RAD;
  Serial.println(vel);
  ///////////////////////////////////////////////
  ////////// PID control ////////////////////////
  float target_vel = goal_left;
  float kp = 150;
  float ki = 100;
  float kd = 1;
  float new_pwr = computePID(kp,ki,kd, target_vel, vel, deltaT);
  last_error = error;
  ///////////////////////////////////////////////

  // Low-pass filter (25 Hz cutoff)
  velFilt = 0.854*velFilt + 0.0728*vel + 0.0728*velPrev;
  velPrev = vel;
  
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
  if(newTime-oldTime>PUB_PERIOD){
    goal_left_msg.data = goal_left;
    goal_left_pub.publish(&goal_left_msg);

    current_vel_msg.data = vel;
    current_vel_pub.publish(&current_vel_msg);

    pwr_msg.data = new_pwr;
    pwr_pub.publish(&pwr_msg);
    
  }

  
  delay(10);
}
float computePID(float kp,float ki,float kd, float target_vel, float velFilt, float deltaT){
  error = target_vel - velFilt;
  cumuError += error*deltaT;
  diffError = (error-last_error)/deltaT;
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
