

// Import ros-related libraries
#include <ros/ros.h>
#include <ros/time.h>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2_ros/transform_broadcaster.h>
#include <geometry_msgs/TransformStamped.h>
#include <turtlesim/Pose.h>

#include <nav_msgs/Odometry.h>
#include <std_msgs/String.h>
#include <sensor_msgs/JointState.h>
#include <geometry_msgs/Quaternion.h>
#include <geometry_msgs/Vector3.h>
#include <geometry_msgs/Pose2D.h>
#include <geometry_msgs/Twist.h>

// useful libraries
#include <string>
#include <vector>
#include <math.h>

// Define Wheel Constant
#define WHEEL_NUM 2
#define WHEEL_RAD 0.125
#define WHEEL_DIST 0.547
#define TICKS_PER_REV 1600

class OdomJointState
{

private:
    ros::NodeHandle nh;
    ros::Publisher odom_pub = nh.advertise<nav_msgs::Odometry>("odom", 100);
    ros::Publisher joint_state_pub = nh.advertise<sensor_msgs::JointState>("joint_states", 100);
    ros::Subscriber ticks_sub = nh.subscribe("ticks", 100, &OdomJointState::ticks_cb, this);
    ros::Subscriber vel_sub = nh.subscribe("vel", 100, &OdomJointState::vel_cb, this);
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
    double v_0 = 0.0;
    double v_1 = 0.0;

    char base_link_name[10] = "base_link";

    float rad[WHEEL_NUM] = {0.0, 0.0};
    std::vector<double> joint_states_pos{0.0, 0.0, 0.0, 0.0};
    std::vector<double> joint_states_vel{0.0, 0.0, 0.0, 0.0};
    int prevPosOdom[WHEEL_NUM] = {0, 0};
    int deltaPos[WHEEL_NUM] = {0, 0};

public:
    sensor_msgs::JointState joint_states;
    tf2_ros::TransformBroadcaster odom_broadcaster;

    nav_msgs::Odometry odom;
    geometry_msgs::TransformStamped odom_trans;

    OdomJointState()
    {
        // pass this
        int z;
        
    }

    void ticks_cb(const geometry_msgs::Vector3::ConstPtr& msg)
    {
        x = msg->x;
        y = msg->y;
        y = msg->z;

        ros::Time t = ros::Time::now();
        calOdom(t);
        calJointStates(t);
    }

    void vel_cb(const geometry_msgs::Pose2D::ConstPtr& msg)
    {
        v_0 = msg->x;
        v_1 = msg->y;

        v_0 = v_0 * 2 * M_PI * WHEEL_RAD / 60.0;
        v_1 = v_1 * 2 * M_PI * WHEEL_RAD / 60.0;
    }

    void calJointStates(ros::Time t)
    {

        joint_states.header.stamp = t;

        // header
        joint_states.header.frame_id = base_link_name;

        joint_states.name[0] = "realsense_yaw_joint";
        joint_states.name[1] = "realsense_pitch_joint";
        joint_states.name[2] = "left_back_wheel_joint";
        joint_states.name[3] = "right_back_wheel_joint";

        // position and velocity

        deltaPos[0] = x - prevPosOdom[0];
        deltaPos[1] = prevPosOdom[1] - y;
        prevPosOdom[0] = x;
        prevPosOdom[1] = y;

        rad[0] = deltaPos[0] * 2 * M_PI / TICKS_PER_REV;
        rad[1] = deltaPos[1] * 2 * M_PI / TICKS_PER_REV;

        joint_states_pos[2] += rad[0];
        joint_states_pos[3] += rad[1];

        joint_states_vel[2] = v_0;
        joint_states_vel[3] = v_1;

        for (int i = 0; i < 4; i++)
        {
            joint_states.position[i] = joint_states_pos[i];
            joint_states.velocity[i] = joint_states_vel[i];
        }
    }
    void calOdom(ros::Time t)
    {

        char odom_child_frame[30] = "base_footprint";
        char odom_header_frame[5] = "odom";
        float last_theta = 0.0;
        float odom_pose[3] = {0.0, 0.0, 0.0};

        odom.header.frame_id = odom_header_frame;
        odom.child_frame_id = odom_child_frame;

        odom_trans.header.frame_id = odom_header_frame;
        odom_trans.child_frame_id = odom_child_frame;

        float delta_s = WHEEL_RAD * (rad[0] + rad[1]) / 2.0;
        float theta = WHEEL_RAD * (rad[1] - rad[0]) / WHEEL_DIST;

        float delta_theta = theta - last_theta;

        odom_pose[0] += delta_s * cos(odom_pose[2] + (delta_theta / 2.0));
        odom_pose[1] += delta_s * sin(odom_pose[2] + (delta_theta / 2.0));
        odom_pose[2] += delta_theta;

        odom_trans.transform.translation.x = odom_pose[0];
        odom_trans.transform.translation.y = odom_pose[1];

        tf2::Quaternion q;
        q.setRPY(0, 0, odom_pose[2]);

        odom_trans.transform.rotation.x = q.x();
        odom_trans.transform.rotation.y = q.y();
        odom_trans.transform.rotation.z = q.z();
        odom_trans.transform.rotation.w = q.w();

        odom_trans.header.stamp = t;

        odom.pose.pose.position.x = odom_pose[0];
        odom.pose.pose.position.y = odom_pose[1];
        // odom.pose.pose.position.z = 0.4021;
        odom.pose.pose.orientation = odom_trans.transform.rotation;
        odom.header.stamp = t;

        double v_x = ((v_0 + v_1) / 2);
        double v_theta = ((v_0 - v_1) / WHEEL_DIST);

        odom.twist.twist.linear.x = v_x;
        odom.twist.twist.angular.z = v_theta;
    }
    void publish_msg()
    {
        joint_state_pub.publish(joint_states);
        odom_broadcaster.sendTransform(odom_trans);
        odom_pub.publish(odom);
    }
};

int main(int argc, char **argv)
{

    ros::init(argc, argv, "odom_jointstate");
    OdomJointState oj;
    ros::Rate loop_rate(20); // 20Hz

    while (ros::ok())
    {
        oj.publish_msg();
        ros::spinOnce();
        loop_rate.sleep();
    }

    return 0;
}
