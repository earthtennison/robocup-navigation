/*
 * rosserial PubSub Example
 * Prints "hello world!" and toggles led
 */
#include <Encoder.h>
#include <ros.h>
#include <std_msgs/Float32.h>
#include <Servo.h>
#include <math.h>

Encoder enc1(2,3);
Servo mobileBase_l_1,mobileBase_l_2;
float feedVx = 0;
float refVx = 0;
int sign=1;
std_msgs::Float32 current_vel_msg;
ros::Publisher current_vel_pub("current_vel", &current_vel_msg);

long oldX = -999;
unsigned long oldTime = 0;

float goal_left = 0; 
float diff_goal = 0;

ros::NodeHandle  nh;

#define WHEEL_DIST 0.547
#define WHEEL_RAD 0.125
#define PUB_PERIOD 100

//std_msgs::UInt16 uint16;/
//ros::Publisher servel("servel", &uint1/6);


void MotorCb( const std_msgs::Float32& float_msg){ // 10-170

  unsigned long newTime = millis();
  
  long newX = enc1.read();
  if (newX != oldX){
    feedVx = map(abs(newX-oldX),0,1000,0,90)*500/3/(newTime-oldTime);//3.14/180*WHEEL_RAD*1000/(newTime-oldTime);
//    int8_t sign;
//    if (newX-oldX<0) sign = -1;
//    else sign = 1;
    oldTime = newTime;
    oldX = newX;
  }
  else{
    feedVx=0;
  }
  mobileBase_l_1.write(float_msg.data);
  mobileBase_l_2.write(float_msg.data);

  current_vel_msg.data = feedVx;

  current_vel_pub.publish(&current_vel_msg); 

}


ros::Subscriber<std_msgs::Float32> sub("motor_power", MotorCb );


void setup()
{
//  Serial.begin(57600);
  nh.getHardware()->setBaud(57600);
  nh.initNode();
  nh.advertise(current_vel_pub);
  nh.subscribe(sub);
  mobileBase_l_1.attach(9);
  mobileBase_l_2.attach(10);
}

void loop()
{
  
  nh.spinOnce();
  
}
