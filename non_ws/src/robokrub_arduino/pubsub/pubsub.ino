/*
 * rosserial PubSub Example
 * Prints "hello world!" and toggles led
 */
#include <Encoder.h>
#include <ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Empty.h>
//#include <std_msgs/UInt16.h>/
#include <Servo.h>
#include <math.h>
#include <geometry_msgs/Twist.h>

Encoder enc1(2,3);
Servo mobileBase_l_1,mobileBase_l_2;
float feedVx = 0;
float refVx = 0;
int sign=1;
geometry_msgs::Twist cmd_vel_rc100_msg;
geometry_msgs::Twist goal_left_msg;
geometry_msgs::Twist current_vel_msg;
ros::Publisher cmd_vel_rc100_pub("cmd_vel_rc100", &cmd_vel_rc100_msg);
ros::Publisher goal_left_pub("goal_left", &goal_left_msg);
ros::Publisher current_vel_pub("current_vel", &current_vel_msg);

long oldX = -999;
unsigned long oldTime = 0;

float goal_left = 0; 

ros::NodeHandle  nh;

//#define MOBILEBASE_L_1 9;
//#define MOBILEBASE_L_2 10;
//#define max_speed_x 1.2
//#define min_speed_x -1.2
#define WHEEL_DIST 0.547
#define WHEEL_RAD 0.125

//std_msgs::UInt16 uint16;/
//ros::Publisher servel("servel", &uint1/6);


void TwistCb( const geometry_msgs::Twist& twist_msg){ // 10-170
  
  float x = twist_msg.linear.x;
  float z = twist_msg.angular.z;

  float goal_left = x-(z*WHEEL_DIST/2);
  float goal_right = x+(z*WHEEL_DIST/2);
  
  goal_left_msg.linear.x  = goal_left;
//  cmd_vel_rc100_msg.angular.z = ;
  
  goal_left_pub.publish(&goal_left_msg);

  long newX = enc1.read();
  unsigned long newTime = millis();
  if (newX != oldX){
    feedVx = map(abs(newX-oldX),0,1000,0,90)*3.14/180*125/(newTime-oldTime);
    int8_t sign;
    if (newX-oldX<0) sign = -1;
    else sign = 1;
//    Serial.print("Difference: "); Serial.println(map(newX-oldX,-200,200,-90,90));
//    Serial.print("Interval: "); Serial.println(newTime-oldTime);
    oldTime = newTime;
    oldX = newX;
//    Serial.print("Speed: "); Serial.println(sign*goal_left);
  }
  else{
    feedVx=0;
  }
  if (abs(feedVx) < abs(goal_left)-0.01){
    nh.loginfo("NOT ENOUGH");
    if (sign==1){
      nh.loginfo("ADD SPEED");
      refVx = refVx+0.01;
    }
    else{
      nh.loginfo("DECREASE SPEED");
      refVx = refVx-0.01;
    }
    
  }
  
  else if(abs(feedVx) > abs(goal_left)+0.01){
    nh.loginfo("TOO MUCH");
    if (sign==1){
      nh.loginfo("DECREASE SPEED");
      refVx = refVx-0.01;
    }
    else{
      nh.loginfo("ADD SPEED");
      refVx = refVx+0.01;
    }
  }
  mobileBase_l_1.write(map(refVx,-3,3,10,170));
  mobileBase_l_2.write(map(refVx,-3,3,10,170));
  cmd_vel_rc100_msg.linear.x  = refVx;
  current_vel_msg.linear.x = feedVx;
//  cmd_vel_rc100_msg.angular.z = ;

  cmd_vel_rc100_pub.publish(&cmd_vel_rc100_msg); 
  current_vel_pub.publish(&current_vel_msg); 

}

ros::Subscriber<geometry_msgs::Twist> sub("cmd_vel", TwistCb );



std_msgs::String str_msg;
ros::Publisher chatter("chatter", &str_msg);


char hello[13] = "hello world!";

void setup()
{
//  Serial.begin(57600);
  nh.getHardware()->setBaud(57600);
  nh.initNode();
//  nh.advertise(chatter);
  nh.advertise(cmd_vel_rc100_pub);
  nh.advertise(goal_left_pub);
  nh.advertise(current_vel_pub);
  nh.subscribe(sub);
  mobileBase_l_1.attach(9);
  mobileBase_l_2.attach(10);
}

void loop()
{
  
  nh.spinOnce();
  

//  else if(sign*feedVx > goal_left){
//    if (sign==1){
//      feedVx = feedVx-0.05;
//      mobileBase_l_1.write(map(feedVx,-3.22,3.22,10,170));
//      mobileBase_l_2.write(map(feedVx,-3.22,3.22,10,170));
//    }
//    else{
//      feedVx = feedVx+0.05;
//      mobileBase_l_1.write(map(feedVx,-3.22,3.22,10,170));
//      mobileBase_l_2.write(map(feedVx,-3.22,3.22,10,170));
//    }
//  }

  delay(500);
  
}
