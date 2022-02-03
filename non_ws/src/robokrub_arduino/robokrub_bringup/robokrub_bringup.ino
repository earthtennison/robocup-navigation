#include <Servo.h>
#include <Encoder.h>
#include <ros.h>
#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/Twist.h>
#include <ros/time.h>
#include <tf/tf.h>
#include <tf/transform_broadcaster.h>
#include <sensor_msgs/JointState.h>
#include <geometry_msgs/Quaternion.h>
#include <SoftwareSerial.h>
#include <ros/time.h>
#include <math.h>
#include <util/atomic.h>


#define WHEEL_NUM 2
#define WHEEL_RAD 0.125
#define WHEEL_DIST 0.547

#define ENCA1 2 
#define ENCA2 3

#define ENCB1 5
#define ENCB2 6

#define Kp 1
#define Ki 1
#define Kd 0

#define mobileBaseL1Pin 7
#define mobileBaseL2Pin 8
#define mobileBaseR1Pin 9
#define mobileBaseR2Pin 10

#define PUB_PERIOD 100

Servo mobileBaseL1, mobileBaseL2, mobileBaseR1, mobileBaseR2;

unsigned long currTime = 0;
unsigned long prevTime = 0;

double Vx = 0 ;
double Vy = 0 ;
double W = 0 ;
double x = 0.0;
double y = 0.0;
double th = 0.0;
//ros::Time current_time, last_time;
//current_time = ros::Time::now();
//last_time = ros::Time::now();

unsigned long oldTime_odom = 0;
unsigned long publishTime = 0;

unsigned long currT[2] = {0, 0};
unsigned long prevT[2] = {0, 0};
int posPrev[2] = {0, 0};
int pos[2] = {0, 0}; 
int deltaPos[2] = {0, 0};
float rad[2] = {0.0, 0.0}; 
// Use the "volatile" directive for variables
// used in an interrupt
volatile int pos_i[2] = {0, 0};

float v[2] = {0.0, 0.0};
float vFilt[2] = {0.0, 0.0};
float vPrev[2] = {0.0, 0.0};
float deltaT = 0;
float goal[2] = {0.0, 0.0};
float e[2] = {0.0, 0.0};
float prevE[2] = {0.0, 0.0}; 
float eInt[2] = {0.0, 0.0};
int pwr[2] = {0, 0};
float odom_pose[3] = {0.0, 0.0, 0.0};
float last_theta = 0.0;
int deltaPose[2] = {0, 0};



ros::NodeHandle  nh;
nav_msgs::Odometry odom;
ros::Publisher odom_pub("odom", &odom);
tf::TransformBroadcaster odom_broadcaster;
geometry_msgs::TransformStamped odom_trans;


sensor_msgs::JointState joint_states;
ros::Publisher joint_states_pub("joint_states", &joint_states);
static char *joint_states_name[2] = {"left_back_wheel_joint", "right_back_wheel_joint"};
float eff[2];

void GoalCb(const geometry_msgs::Twist& twist_msg){
  
  float x = twist_msg.linear.x;
  float z = twist_msg.linear.z;

  goal[0] = x-z*WHEEL_DIST/2;
  goal[1] = x+z*WHEEL_DIST/2;
}

ros::Subscriber<geometry_msgs::Twist> sub("cmd_vel", TwistCb );

void setup() {
  
  pinMode(ENCA1, INPUT_PULLUP);
  pinMode(ENCA2, INPUT_PULLUP);
  pinMode(ENCB1, INPUT_PULLUP); 
  pinMode(ENCB2, INPUT_PULLUP);
  
  
  nh.getHardware()->setBaud(57600);
  nh.initNode();
  nh.advertise(odom_pub);
  nh.advertise(joint_states_pub);
  nh.subscribe(sub);
  odom_broadcaster.init(nh);
  joint_states.header.frame_id = "base_link";
  joint_states.name = joint_states_name;

  mobileBaseL1.attach(mobileBaseL1Pin);
  mobileBaseL2.attach(mobileBaseL2Pin);
  mobileBaseR1.attach(mobileBaseR1Pin);
  mobileBaseR2.attach(mobileBaseR2Pin);

  mobileBaseL1.write(98);
  mobileBaseL2.write(98);
  mobileBaseR1.write(98);
  mobileBaseR2.write(98);

  attachInterrupt(digitalPinToInterrupt(ENCA1),readEncoderLeft,RISING);
  attachInterrupt(digitalPinToInterrupt(ENCB1),readEncoderRight,RISING);
  
}
void loop() {

  currTime = millis();
  
  motorControl(ENCA1);
  motorControl(ENCB1);
  
  Vx=v[0]+(v[1]-v[0])/2;
  W=v[0]/((v[0]*WHEEL_DIST)/(v[0]+v[1]));

  calJointStates();
  calOdom();
  if(currTime-prevTime > PUB_PERIOD){
    publishState();
  }
  nh.spinOnce();
}

void motorControl(int pin){

  int num = 0;
  if(pin == ENCB1){
    num = 1;
  }
  
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
    pos[num] = pos_i[num];
  }

  currT[num] = micros();
  float deltaT = ((float) (currT[num]-prevT[num]))/1.0e6;
  deltaPose[num] = pos[num]-posPrev[num];
  float vel = (deltaPose[num])/deltaT;
  posPrev[num] = pos[num];
  prevT[num] = currT[num];
  

  // Convert count/s to RPM
  v[num] = vel/400.0*60.0;


  // Low-pass filter (25 Hz cutoff)
  vFilt[num] = 0.854*vFilt[num] + 0.0728*v[num] + 0.0728*vPrev[num];
  vPrev[num] = v[num];
  
  // Compute the control signal u
  computePID(num, deltaT);
  
}

void computePID(int num, float deltaT){
  e[num] = goal[num]-v[num];
  float diffE = (e[num]-prevE[num])/deltaT;
  eInt[num] = eInt[num] + e[num]*deltaT;
  float u = Kp*e[num] + Ki*eInt[num]+Kd*diffE;
  prevE[num] = e[num];
//  Serial.println(u);
  // Set the motor speed and direction
  int dir = -1;
  if (u<0){
    dir = 1;
  }
  pwr[num] = (int) fabs(u);
  if(pwr[num] > 80){
    pwr[num] = 80;
  }
  setMotor(num, dir,pwr[num]);
}

void setMotor(int num, int dir, int pwr){
  if(num == 0){
    mobileBaseL1.write(90+(dir*pwr));
    mobileBaseL2.write(90+(dir*pwr));
  }
  else{
    mobileBaseR1.write(90+(dir*pwr));
    mobileBaseR2.write(90+(dir*pwr));
  }
}

void calOdom(){
  
    float delta_s = WHEEL_RAD * (rad[0]+rad[1])/2.0;
    float theta = WHEEL_RAD * (rad[1]-rad[0])/WHEEL_DIST;

    float delta_theta = theta-last_theta;
    
    odom_pose[0] = delta_s * cos(odom_pose[2] + (delta_theta / 2.0));
    odom_pose[1] = delta_s * sin(odom_pose[2] + (delta_theta / 2.0));
    odom_pose[2] = delta_theta;
    
    
    odom_trans.header.frame_id = "odom";
    odom_trans.child_frame_id = "base_footprint";
    odom_trans.transform.translation.x = odom_pose[0];
    odom_trans.transform.translation.y = odom_pose[1];
    odom_trans.transform.translation.z = 0.0;
    odom_trans.transform.rotation = tf::createQuaternionFromYaw(odom_pose[2]); ;
    odom_trans.header.stamp = nh.now();

    odom.pose.pose.position.x = odom_pose[0];
    odom.pose.pose.position.y = odom_pose[1];
    odom.pose.pose.position.z = 0.0;
    odom.pose.pose.orientation = odom_trans.transform.rotation;
    odom.child_frame_id = "base_footprint";
    odom.twist.twist.linear.x = Vx;
    odom.twist.twist.angular.z = W;
}
void calJointStates(){
    rad[0] = deltaPose[0]*2*M_PI/400.0;
    rad[1] = deltaPose[1]*2*M_PI/400.0;

    joint_states.position = rad;
    joint_states.velocity = v;
    joint_states.header.stamp=nh.now();
}
void publishState(){
  calJointStates();
  calOdom();
  odom_broadcaster.sendTransform(odom_trans);
  odom_pub.publish(&odom);
  joint_states_pub.publish(&joint_states);
}
void readEncoderLeft(){
  // Read encoder B when ENCA rises
  int b = digitalRead(ENCA2);
  int increment = 0;
  if(b>0){
    // If B is high, increment forward
    increment = -1;
  }
  else{
    // Otherwise, increment backward
    increment = 1;
  }
  pos_i[0] = pos_i[0] + increment;
  // Compute velocity with method 2
}
void readEncoderRight(){
  // Read encoder B when ENCA rises
  int b = digitalRead(ENCB2);
  int increment = 0;
  if(b>0){
    // If B is high, increment forward
    increment = -1;
  }
  else{
    // Otherwise, increment backward
    increment = 1;
  }
  pos_i[1] = pos_i[1] + increment;
  // Compute velocity with method 2
}
