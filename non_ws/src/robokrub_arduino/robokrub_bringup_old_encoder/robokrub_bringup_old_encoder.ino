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
#define WHEEL_NUM 2
#define WHEEL_RAD 0.125
double Vx = 0 ;
double Vy = 0 ;
double W = 0 ;
double L = 0.547;
double x = 0.0;
double y = 0.0;
double th = 0.0;
//ros::Time current_time, last_time;
//current_time = ros::Time::now();
//last_time = ros::Time::now();
Encoder enc1(2,3);
Encoder enc2(4,5);
unsigned long oldTime_enc1 = 0;
unsigned long oldTime_enc2 = 0;
unsigned long oldTime_odom = 0;
unsigned long publishTime = 0;
int x1 = A0;
int y1 = A1;
long oldX1 = -999;
uint16_t oldReadVX1= 1790;
uint16_t refVX1;
uint16_t feedVX1;
int x2 = A2;
int y2 = A3;
long oldX2 = -999;
uint16_t oldReadVX2= 1790;
uint16_t refVX2;
uint16_t feedVX2;
double V1;
double V2;
ros::NodeHandle  nh;
nav_msgs::Odometry odom;
ros::Publisher odom_pub("odom", &odom);
tf::TransformBroadcaster odom_broadcaster;
sensor_msgs::JointState joint_states;
ros::Publisher joint_states_pub("joint_states", &joint_states);
static char *joint_states_name[2] = {"left_back_wheel_joint", "right_back_wheel_joint"};
float pos[2];
float vel[2];
float eff[2];
float left_rad = 0;
float right_rad= 0;
void setup() {
  nh.getHardware()->setBaud(57600);
  nh.initNode();
  nh.advertise(odom_pub);
  nh.advertise(joint_states_pub);
  joint_states.header.frame_id = "bask_link";
  joint_states.name = joint_states_name;
}
void loop() {
  //encoder1
  encoder(x1 , oldX1 , oldReadVX1 , refVX1 , feedVX1 , V1 , enc1 , oldTime_enc1);
  //encoder2
  encoder(x2 , oldX2 , oldReadVX2 , refVX2 , feedVX2 , V2 , enc2 , oldTime_enc2);
  Vx=V1+(V2-V1)/2;
  W=V1/((V1*L)/(V1+V2));
  calOdom();
  calJointStates();
  publishState();
  nh.spinOnce();
}
void encoder(int x , long oldX , uint16_t oldReadVX , uint16_t refVX , uint16_t feedVX , double V , Encoder enc , unsigned long oldTime){
  uint16_t newReadVX = analogRead(x); // Max Speed = 3580 RPM
  if (abs(newReadVX-oldReadVX)>5){
    refVX = newReadVX;
    oldReadVX = newReadVX;
  }
  long newX = enc.read();
  unsigned long newTime = millis();
  if (newX != oldX){
    feedVX = map(abs(newX-oldX),0,1000,0,90)*3.14/180*WHEEL_RAD*1000/(newTime-oldTime);
    int8_t sign;
    if (newX-oldX<0) sign = -1;
    else sign = 1;
    oldTime = newTime;
    oldX = newX;
    V=sign*feedVX;
    Serial.print("Speed: "); Serial.println(V);
  }
}
void calOdom(){
    unsigned long newTime = millis()*0.001;
    double dt = (newTime - oldTime_odom);
    double delta_x = (Vx * cos(th) - Vy * sin(th)) * dt;
    double delta_y = (Vx * sin(th) + Vy * cos(th)) * dt;
    double delta_th = W * dt;
    x += delta_x;
    y += delta_y;
    th += delta_th;
    float qz = sin(th) * sin(abs(th)/2.0f) ;
    geometry_msgs::TransformStamped odom_trans;
    odom_trans.header.frame_id = "odom";
    odom_trans.child_frame_id = "base_link";
    odom_trans.transform.translation.x = x;
    odom_trans.transform.translation.y = y;
    odom_trans.transform.translation.z = 0.0;
    odom_trans.transform.rotation.z = qz  ;
    odom.header.stamp = nh.now();
    odom_broadcaster.sendTransform(odom_trans);
    odom.pose.pose.position.x = x;
    odom.pose.pose.position.y = y;
    odom.pose.pose.position.z = 0.0;
    odom.pose.pose.orientation.z = qz ;
    odom.child_frame_id = "base_link";
    odom.twist.twist.linear.x = Vx;
    odom.twist.twist.linear.y = Vy;
    odom.twist.twist.angular.z = W;
    oldTime_odom = newTime;
}
void calJointStates(){
    pos[0] = left_rad;
    pos[1] = right_rad;
    vel[0]  = V1;
    vel[1]  = V2;
    joint_states.position = pos;
    joint_states.velocity = vel;
    joint_states.header.stamp=nh.now();
}
void publishState(){
  unsigned long newTime = millis()*0.001;
  if(newTime-publishTime > 1){
      odom_pub.publish(&odom);
      joint_states_pub.publish(&joint_states);
      publishTime = newTime;
  }
}
