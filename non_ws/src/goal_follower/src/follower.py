#!/usr/bin/env python
# license removed for brevity

import rospy

#Library for listening to transform
import math
import tf2_ros
import geometry_msgs.msg

# Brings in the SimpleActionClient
import actionlib
# Brings in the .action file and messages used by the move base action
from move_base_msgs.msg import MoveBaseAction, MoveBaseGoal

def movebase_client(tfBuffer, listener):



   # Create an action client called "move_base" with action definition file "MoveBaseAction"
    client = actionlib.SimpleActionClient('move_base',MoveBaseAction)
 
   # Waits until the action server has started up and started listening for goals.
    client.wait_for_server()

   # Create a rate of 10 s
    rate = rospy.Rate(5)


   # Listen to pose from robot1
    pose = tfBuffer.lookup_transform('map','robot2_tf/base_footprint',rospy.Time.now()-rospy.Duration.from_sec(1.0))


   # Creates a new goal with the MoveBaseGoal constructor
    goal = MoveBaseGoal()
    goal.target_pose.header.frame_id = "map"
    goal.target_pose.header.stamp = rospy.Time.now()-rospy.Duration.from_sec(1.0)
   # Move 0.5 meters forward along the x axis of the "map" coordinate frame 
    goal.target_pose.pose.position.x = pose.transform.translation.x
    goal.target_pose.pose.position.y = pose.transform.translation.y
   # No rotation of the mobile base frame w.r.t. map frame
    goal.target_pose.pose.orientation = pose.transform.rotation

   # Sends the goal to the action server.
    client.send_goal(goal)

   # Waits for the server to finish performing the action.
    wait = client.wait_for_result()
   # If the result doesn't arrive, assume the Server is not available
    if not wait:
        rospy.logerr("Action server not available!")
        rospy.signal_shutdown("Action server not available!")
    else:
    # Result of executing the action
        return client.get_result()   

# If the python node is executed as main process (sourced directly)
if __name__ == '__main__':
    try:
       # Initializes a rospy node to let the SimpleActionClient publish and subscribe
        rospy.init_node('movebase_client_py')
        tfBuffer = tf2_ros.Buffer()
        listener = tf2_ros.TransformListener(tfBuffer)
        rospy.sleep(5)
        while True:
            movebase_client(tfBuffer,listener)
            if result:
                rospy.loginfo("Goal execution done!")
    except rospy.ROSInterruptException:
        rospy.loginfo("Navigation test finished.")
    
