#include <Arduino.h>
#include "robokrub_config.h"

// Time variables
unsigned long prevT_state = 0;

void setup()
{

  // Set up publisher, subscriber and broadcaster for ROS
  nh.getHardware()->setBaud(115200);
  nh.initNode();
  // nh.advertise(debug_pub);
  nh.subscribe(cmd_vel_sub);
  nh.advertise(ticks_pub);
  nh.advertise(vel_pub);
  // nh.subscribe(servo_direction_sub);

  mobileBaseL.attach(mobileBaseLPin);
  mobileBaseR.attach(mobileBaseRPin);

  // camera_pitch_servo.attach(PITCH_PIN);
  // camera_yaw_servo.attach(YAW_PIN);

  // camera_pitch_servo.write(PITCH_INIT_STATE);
  // camera_yaw_servo.write(YAW_INIT_STATE);

  // Initiate PID controller
  PIDController_Init(&pidLeft);
  PIDController_Init(&pidRight);
}
void loop()
{

  // put your main code here, to run repeatedly:
  setMotor(pwr[0], mobileBaseL);
  setMotor(pwr[1], mobileBaseR);

  currT = millis();
  // currT_state = millis();

  pos[0] = encLeft.read();
  pos[1] = encRight.read();

  ticks_msg.x = pos[0];
  ticks_msg.y = pos[1];

  if (currT - prevT > 50)
  {

    // nh.loginfo("1");
    velocity[0] = (pos[0] - posPrev[0]) * 1e3 / ((float)(currT - prevT));
    velocity[1] = (posPrev[1] - pos[1]) * 1e3 / ((float)(currT - prevT));
    // Not sure about this
    velocity[0] = (velocity[0] / TICKS_PER_REV) * 60;
    velocity[1] = (velocity[1] / TICKS_PER_REV) * 60;
    posPrev[0] = pos[0];
    posPrev[1] = pos[1];

    // Calculate PID of each wheel
    pwr[0] = PIDController_Update(&pidLeft, goal[0], velocity[0]);
    pwr[1] = PIDController_Update(&pidRight, goal[1], velocity[1]);

    if ((goal[0] == 0.0) && (goal[1] == 0.0))
    {
      pwr[0] = 0.0;
      pwr[1] = 0.0;
    }

    setMotor(pwr[0], mobileBaseL);
    setMotor(pwr[1], mobileBaseR);
    prevT = currT;
  }

  vel_msg.x = velocity[0];
  vel_msg.y = velocity[1];

  if (currT - prevT_state > 50)
  {

    ticks_pub.publish(&ticks_msg);
    vel_pub.publish(&vel_msg);
    prevT_state = currT;
  }

  // velocity[0] = velocity[0]*2*M_PI*WHEEL_RAD/60.0;
  // velocity[1] = velocity[1]*2*M_PI*WHEEL_RAD/60.0;

  // Vx=((velocity[0]+velocity[1])/2);
  // W=((velocity[0]-velocity[1])/WHEEL_DIST);

  // vel_msg.x = Vx;
  // vel_msg.theta = W;
  // vel_pub.publish(&vel_msg);

  // debug_msg.data = odom_child_frame;
  // debug_pub.publish(&debug_msg);
  nh.spinOnce();
}

void setMotor(int pwr, Servo servo)
{
  servo.write(90 + pwr);
}

void GoalCb(const geometry_msgs::Twist &twist_msg)
{

  float x = twist_msg.linear.x;
  float z = twist_msg.angular.z;

  float goal_left = x - z * WHEEL_DIST / 2;
  float goal_right = x + z * WHEEL_DIST / 2;

  goal_left = goal_left * 60.0 / (2 * M_PI * WHEEL_RAD);
  goal_right = goal_right * 60.0 / (2 * M_PI * WHEEL_RAD);

  if (goal_left > 80.0)
  {
    goal_left = 80.0;
  }
  else if (goal_left < -80.0)
  {
    goal_left = -80.0;
  }

  if (goal_right > 80.0)
  {
    goal_right = 80.0;
  }
  else if (goal_right < -80.0)
  {
    goal_right = -80.0;
  }

  goal[0] = goal_left;
  goal[1] = goal_right;
}
