#!/usr/bin/env python
# license removed for brevity

import rospy
from nav_msgs.msg import Odometry
import tf2_ros

# Brings in the SimpleActionClient
import actionlib
# Brings in the .action file and messages used by the move base action
from move_base_msgs.msg import MoveBaseAction, MoveBaseGoal


class PoseGoal:

    def __init__(self):
        self.stop = False
        self.client = actionlib.SimpleActionClient('move_base',MoveBaseAction)

        self.tfBuffer = tf2_ros.Buffer()
        self.listener = tf2_ros.TransformListener(self.tfBuffer)
        
        rospy.sleep(5)
       
        self.client.wait_for_server()
        rospy.loginfo("Action server is up, we can send new goal!")


    def send_goal_and_get_result(self):

        

        pose = self.tfBuffer.lookup_transform('map','human_frame',rospy.Time.now()-rospy.Duration.from_sec(0.05))

        goal = MoveBaseGoal()
        goal.target_pose.header.frame_id = "map"
        goal.target_pose.header.stamp = rospy.Time.now()-rospy.Duration.from_sec(0.05)
        goal.target_pose.pose.position.x = pose.transform.translation.x
        goal.target_pose.pose.position.y = pose.transform.translation.y
        goal.target_pose.pose.orientation = pose.transform.rotation

        self.client.send_goal(goal)
        
        wait = self.client.wait_for_result()
        if not wait:
            rospy.logerr("Action server not available!")
            rospy.signal_shutdown("Action server not available!")
        else:
            return self.client.get_result()   


if __name__=='__main__':

    rospy.init_node('pose_goal')
    
    client = PoseGoal()


    while not rospy.is_shutdown():
        rospy.sleep(2.5)
        try:
            result = client.send_goal_and_get_result()
            if result:
                rospy.loginfo("Goal execution done!")
        except rospy.ROSInterruptException:
            rospy.loginfo("Navigation test finished.")
        
        
    rospy.spin()
