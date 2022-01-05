/*
 * rosserial PubSub Example
 * Prints "hello world!" and toggles led
 */

#include <ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Empty.h>
//#include <std_msgs/UInt16.h>/
#include <Servo.h>
#include <math.h>
#include <geometry_msgs/Twist.h>

Servo mobileBase_l_1,mobileBase_l_2;

ros::NodeHandle  nh;

#define MOBILEBASE_L_1 9;
#define MOBILEBASE_L_2 10;
#define max_speed_x 1.2
#define min_speed_x -1.2

//std_msgs::UInt16 uint16;/
//ros::Publisher servel("servel", &uint1/6);


void TwistCb( const geometry_msgs::Twist& twist_msg){ // 10-170
  
  float x = twist_msg.linear.x;
  float z = twist_msg.angular.z;

  float l = (x-z)/2;
  float r = (x+z)/2;

  float abl = abs(l);
  float abr = abs(r);

  abl = abl*(80-0)/(4.0);

//  std_msgs::UInt16 servoL1 /;
  int servoL;//

  if(l>0){
//    servoL1 = round(90.0+abl);/
    servoL = round(90.0+abl);//
  }
  else{
//    servoL1 = round(90.0-abl);/
    servoL = round(90.0+abl);//
  }

  mobileBase_l_1.write(servoL);
  mobileBase_l_2.write(servoL);
//  servel.publish(servoL);///
  
}

ros::Subscriber<geometry_msgs::Twist> sub("cmd_vel", TwistCb );



std_msgs::String str_msg;
ros::Publisher chatter("chatter", &str_msg);


char hello[13] = "hello world!";

void setup()
{
  nh.initNode();
  nh.advertise(chatter);
  nh.subscribe(sub);
  mobileBase_l_1.attach(9);
  mobileBase_l_2.attach(10);
}

void loop()
{
  nh.spinOnce();
}
