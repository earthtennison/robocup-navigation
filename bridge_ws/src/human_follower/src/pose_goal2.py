#!/usr/bin/env python
# license removed for brevity

import rospy
from nav_msgs.msg import Odometry
import tf2_ros

# Brings in the SimpleActionClient
import actionlib
# Brings in the .action file and messages used by the move base action
from move_base_msgs.msg import MoveBaseAction, MoveBaseGoal
from std_msgs.msg import Bool


class PoseGoal:

    def __init__(self):
        self.stop = False
        self.client = actionlib.SimpleActionClient('move_base',MoveBaseAction)

        self.tfBuffer = tf2_ros.Buffer()
        self.listener = tf2_ros.TransformListener(self.tfBuffer)
        # self.
        self.target_sub = rospy.Subscriber("target_lost", Bool , self.target_lost)
        
        rospy.sleep(5)
       
        self.client.wait_for_server()
        rospy.loginfo("Action server is up, we can send new goal!")

    def target_lost(self,msg):
        self.target_lost = msg.data
        # if msg.data == True:
        #     executing.data = False
        # else : executing.data = True
        # self.executing_check.publish(executing)


    def send_goal_and_get_result(self):

        if self.target_lost:
            return None
        pose = self.tfBuffer.lookup_transform('map','human_frame',rospy.Time.now()-rospy.Duration.from_sec(1))

       

        rospy.loginfo("X = "+str(pose.transform.translation.x))
        rospy.loginfo("Y = "+str(pose.transform.translation.y))


        goal = MoveBaseGoal()
        goal.target_pose.header.frame_id = "map"
        goal.target_pose.header.stamp = rospy.Time.now()-rospy.Duration.from_sec(1)
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
    executing = Bool()
    executing.data = True

    while not rospy.is_shutdown():
        rospy.sleep(2.5)
        try:
            result = client.send_goal_and_get_result()
            if result:
                rospy.loginfo("Goal execution done!")
                rospy.loginfo("------------------------------")
                executing.data = False
        except rospy.ROSInterruptException:
            rospy.loginfo("Navigation test finished.")
        executing_check = rospy.Publisher("executing", Bool , queue_size=1)
        executing_check.publish(executing)
        
        
    rospy.spin()
