#!/usr/bin/env python3

from cv_bridge import CvBridge, CvBridgeError 
from sensor_msgs.msg import Image 
import rospy
import cv2

def callback(frame):
    cv_image = bridge.imgmsg_to_cv2(frame, "bgr8")
    cv2.imshow("Image window", cv_image)
    cv2.waitKey(3)

    print(cv_image.shape)

if __name__ == '__main__':
    rospy.init_node('image_converter', anonymous=True)
    bridge = CvBridge()
    sub = rospy.Subscriber("/camera/color/image_raw", Image , callback)
    try:
        rospy.spin()
    except KeyboardInterrupt:
        print("Shutting down")
    cv2.destroyAllWindows()
