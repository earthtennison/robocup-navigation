#!/usr/bin/env python

import rospy
import actionlib
from std_msgs.msg import Empty
from my_robot_msgs.msg import MoveRobotAction , MoveRobotGoal

class MoveRobotClient:

    def __init__(self):
        self._ac = actionlib.SimpleActionClient('/move_robot', MoveRobotAction)
        self._ac.wait_for_server()
        rospy.loginfo("Server is up, let's send goal!")

        self.sub = rospy.Subscriber('/cancel_move', Empty, self.cancel_move_callback)



    def cancel_move_callback(self,req):
        rospy.loginfo("Received message to cancel goal")
        self._ac.cancel_goal()

    def send_goal_and_get_result(self):
        goal = MoveRobotGoal()
        goal.position = 95
        goal.velocity = 5
        self._ac.send_goal(goal, done_cb=self.done_callback, feedback_cb=self.feedback_callback)
        rospy.loginfo("Coal has been sent")

    
    def done_callback(self,status,result):
        rospy.loginfo("Status: " + str(status))
        rospy.loginfo("Result: " + str(result))

    def feedback_callback(self, feedback):
        rospy.loginfo(feedback)
        



if __name__=='__main__':
    rospy.init_node('move_robot_client')

    client = MoveRobotClient()

    client.send_goal_and_get_result()

    rospy.spin()