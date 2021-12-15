#!/usr/bin/env python  
import rospy
import tf2_ros
import tf
import tf2_msgs.msg
import geometry_msgs.msg
from geometry_msgs.msg import PoseStamped


class FixedTFBroadcaster:


    def __init__(self):
        self.pub_tf = rospy.Publisher("/tf", tf2_msgs.msg.TFMessage, queue_size=1)
        self.call_position = rospy.Subscriber('/coordinate', PoseStamped, self.set_pose)
        self.distance = PoseStamped()

        while not rospy.is_shutdown():
            # Run this loop at about 10Hz
            rospy.sleep(0.1)

            t = geometry_msgs.msg.TransformStamped()
            t.header.frame_id = "base_footprint"
            t.header.stamp = rospy.Time.now()
            t.child_frame_id = "/human_frame"
            t.transform.translation.x = self.distance.pose.position.x
            t.transform.translation.y = self.distance.pose.position.y
            t.transform.translation.z = 0.0

            quat = tf.transformations.quaternion_from_euler(float(0),float(0),float(0))

            t.transform.rotation.x = quat[0]
            t.transform.rotation.y = quat[1]
            t.transform.rotation.z = quat[2]
            t.transform.rotation.w = quat[3]

            tfm = tf2_msgs.msg.TFMessage([t])
            self.pub_tf.publish(tfm)

    def set_pose(self,msg):
        self.distance.pose.position.x = msg.pose.position.x
        self.distance.pose.position.y = msg.pose.position.y
       
        

if __name__ == '__main__':
    rospy.init_node('human_frame_run')
    tfb = FixedTFBroadcaster()

    rospy.spin()