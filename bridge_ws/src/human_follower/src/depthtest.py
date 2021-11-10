#!/usr/bin/env python3

from cv_bridge import CvBridge, CvBridgeError 
from sensor_msgs.msg import Image 
import rospy
import cv2
import numpy as np
import matplotlib.pyplot as plt

def callback(frame):
    depth_image = bridge.imgmsg_to_cv2(frame, "passthrough")

    depth_array = np.array(depth_image, dtype=np.float32)

    cv2.imshow('Depth',depth_image)
    print(depth_image)

if __name__ == '__main__':
    rospy.init_node('image_converter', anonymous=True)
    bridge = CvBridge()
    sub = rospy.Subscriber("/camera/aligned_depth_to_color/image_raw", Image , callback)
    try:
        rospy.spin()
    except KeyboardInterrupt:
        print("Shutting down")
    cv2.destroyAllWindows()
