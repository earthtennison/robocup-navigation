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
        self.target_lost = False
        self.tfBuffer = tf2_ros.Buffer()
        self.listener = tf2_ros.TransformListener(self.tfBuffer)
        self.pub_executing = rospy.Publisher("executing", Bool , queue_size=10)
        self.target_sub = rospy.Subscriber("target_lost", Bool , self.target_lost_callback)
        
        self.executing = Bool()
        self.executing.data = True
        
        rospy.sleep(5)
       
        self.client.wait_for_server()
        rospy.loginfo("Action server is up, we can send new goal!")

    def target_lost_callback(self,msg):
        self.target_lost = msg.data
        if self.target_lost == False: 
            self.executing.data = True


    def send_goal_and_get_result(self):

        pose = self.tfBuffer.lookup_transform('map','human_frame',rospy.Time.now()-rospy.Duration.from_sec(1))
        self.pub_executing.publish(self.executing)

        


        goal = MoveBaseGoal()
        goal.target_pose.header.frame_id = "map"
        goal.target_pose.header.stamp = rospy.Time.now()-rospy.Duration.from_sec(1)
        goal.target_pose.pose.position.x = pose.transform.translation.x
        goal.target_pose.pose.position.y = pose.transform.translation.y
        goal.target_pose.pose.orientation = pose.transform.rotation

        if not (self.target_lost) and self.executing.data == True:
            self.client.send_goal(goal)
            rospy.loginfo("X = "+str(pose.transform.translation.x))
            rospy.loginfo("Y = "+str(pose.transform.translation.y))

        
        
        if self.target_lost == True and self.executing.data == True:
            wait = self.client.wait_for_result()
            self.executing.data = False
            if not wait:
                rospy.logerr("Action server not available!")
                rospy.signal_shutdown("Action server not available!")
            else:
                print("Target lost!")
                return self.client.get_result()
        elif self.executing.data == True:
            wait = self.client.wait_for_result(rospy.Duration.from_sec(15))
            self.executing.data = True
        
            


if __name__=='__main__':

    rospy.init_node('pose_goal')
    
    client = PoseGoal()
   

    while not rospy.is_shutdown():
        # rospy.sleep(2.5)
        try:
            result = client.send_goal_and_get_result()
            if result:
                rospy.loginfo("Goal execution done!")
                rospy.loginfo("------------------------------")
        except rospy.ROSInterruptException:
            rospy.loginfo("Navigation test finished.")
        
        
        
        
    rospy.spin()