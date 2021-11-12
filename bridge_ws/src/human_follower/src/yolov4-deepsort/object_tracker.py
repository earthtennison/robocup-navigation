#!/usr/bin/env python3
import os
import rospy

# comment out below line to enable tensorflow logging outputs
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '3'
import time
import tensorflow as tf
physical_devices = tf.config.experimental.list_physical_devices('GPU')
if len(physical_devices) > 0:
    tf.config.experimental.set_memory_growth(physical_devices[0], True)
from absl import app, flags, logging
from absl.flags import FLAGS
import core.utils as utils
from core.yolov4 import filter_boxes
from tensorflow.python.saved_model import tag_constants
from core.config import cfg
from PIL import Image
import cv2
import numpy as np
import matplotlib.pyplot as plt
from tensorflow.compat.v1 import ConfigProto
from tensorflow.compat.v1 import InteractiveSession
# deep sort imports
from deep_sort import preprocessing, nn_matching
from deep_sort.detection import Detection
from deep_sort.tracker import Tracker
from tools import generate_detections as gdet

from cv_bridge import CvBridge, CvBridgeError #
from sensor_msgs.msg import Image, CameraInfo #

#Import realsense2 library
import pyrealsense2.pyrealsense2 as rs2

#Setting variables for person following
global found_target
found_target = False
global target_ID
target_ID = None
# global SLEEP_TIME = 5
global x_pixel
x_pixel = None
global y_pixel
y_pixel = None
global intrinsics
intrinsics = None
global x_coord
x_coord = None
global y_coord
y_coord = None



rospy.set_param('framework', 'tf')
rospy.set_param('weights', './checkpoints/yolov4-tiny-416')
rospy.set_param('size', 416)
rospy.set_param('tiny', True)
rospy.set_param('model', 'yolov4')
rospy.set_param('video', './data/video/test.mp4')
# rospy.set_param('output', None)
# rospy.set_param('output_format', 'XVID')
rospy.set_param('iou', 0.45)
rospy.set_param('score', 0.50)
rospy.set_param('dont_show', False)
rospy.set_param('info',True)
rospy.set_param('count', False)

# flags.DEFINE_string('framework', 'tf', '(tf, tflite, trt')
# flags.DEFINE_string('weights', './checkpoints/yolov4-416',
#                     'path to weights file')
# flags.DEFINE_integer('size', 416, 'resize images to')
# flags.DEFINE_boolean('tiny', False, 'yolo or yolo-tiny')
# flags.DEFINE_string('model', 'yolov4', 'yolov3 or yolov4')
# flags.DEFINE_string('video', './data/video/test.mp4', 'path to input video or set to 0 for webcam')
# flags.DEFINE_string('output', None, 'path to output video')
# flags.DEFINE_string('output_format', 'XVID', 'codec used in VideoWriter when saving video to file')
# flags.DEFINE_float('iou', 0.45, 'iou threshold')
# flags.DEFINE_float('score', 0.50, 'score threshold')
# flags.DEFINE_boolean('dont_show', False, 'dont show video output')
# flags.DEFINE_boolean('info', False, 'show detailed info of tracked objects')
# flags.DEFINE_boolean('count', False, 'count objects being tracked on screen')

def yolo_callback(frame):
    global x_pixel
    global y_pixel
    global z_pixel
    global found_target
    global target_ID
    global SLEEP_TIME
    global frame_num
    frame = bridge.imgmsg_to_cv2(frame, "bgr8")
    frame_num = frame_num+1
    print('Frame #: ', frame_num)
    frame_size = frame.shape[:2]
    image_data = cv2.resize(frame, (input_size, input_size))
    image_data = image_data / 255.
    image_data = image_data[np.newaxis, ...].astype(np.float32)
    start_time = time.time()

    # run detections on tflite if flag is set
    if rospy.get_param('framework') == 'tflite':
        interpreter.set_tensor(input_details[0]['index'], image_data)
        interpreter.invoke()
        pred = [interpreter.get_tensor(output_details[i]['index']) for i in range(len(output_details))]
        # run detections using yolov3 if flag is set
        if rospy.get_param('model')== 'yolov3' and rospy.get_param('tiny') == True:
            boxes, pred_conf = filter_boxes(pred[1], pred[0], score_threshold=0.25,
                                            input_shape=tf.constant([input_size, input_size]))
        else:
            boxes, pred_conf = filter_boxes(pred[0], pred[1], score_threshold=0.25,
                                            input_shape=tf.constant([input_size, input_size]))
    else:
        batch_data = tf.constant(image_data)
        pred_bbox = infer(batch_data)
        for key, value in pred_bbox.items():
            boxes = value[:, :, 0:4]
            pred_conf = value[:, :, 4:]

    boxes, scores, classes, valid_detections = tf.image.combined_non_max_suppression(
        boxes=tf.reshape(boxes, (tf.shape(boxes)[0], -1, 1, 4)),
        scores=tf.reshape(
            pred_conf, (tf.shape(pred_conf)[0], -1, tf.shape(pred_conf)[-1])),
        max_output_size_per_class=50,
        max_total_size=50,
        iou_threshold=rospy.get_param('iou'),
        score_threshold=rospy.get_param('score')
    )

    # convert data to numpy arrays and slice out unused elements
    num_objects = valid_detections.numpy()[0]
    bboxes = boxes.numpy()[0]
    bboxes = bboxes[0:int(num_objects)]
    scores = scores.numpy()[0]
    scores = scores[0:int(num_objects)]
    classes = classes.numpy()[0]
    classes = classes[0:int(num_objects)]

    # format bounding boxes from normalized ymin, xmin, ymax, xmax ---> xmin, ymin, width, height
    original_h, original_w, _ = frame.shape
    bboxes = utils.format_boxes(bboxes, original_h, original_w)

    # store all predictions in one parameter for simplicity when calling functions
    pred_bbox = [bboxes, scores, classes, num_objects]

    # read in all class names from config
    class_names = utils.read_class_names(cfg.YOLO.CLASSES)

    # by default allow all classes in .names file
    allowed_classes = list(class_names.values())
    
    # custom allowed classes (uncomment line below to customize tracker for only people)
    allowed_classes = ['person']

    # loop through objects and use class index to get class name, allow only classes in allowed_classes list
    names = []
    deleted_indx = []
    for i in range(num_objects):
        class_indx = int(classes[i])
        class_name = class_names[class_indx]
        if class_name not in allowed_classes:
            deleted_indx.append(i)
        else:
            names.append(class_name)
    names = np.array(names)
    count = len(names)
    if rospy.get_param('count'):
        cv2.putText(frame, "Objects being tracked: {}".format(count), (5, 35), cv2.FONT_HERSHEY_COMPLEX_SMALL, 2, (0, 255, 0), 2)
        print("Objects being tracked: {}".format(count))
    # delete detections that are not in allowed_classes
    bboxes = np.delete(bboxes, deleted_indx, axis=0)
    scores = np.delete(scores, deleted_indx, axis=0)

    # encode yolo detections and feed to tracker
    features = encoder(frame, bboxes)
    detections = [Detection(bbox, score, class_name, feature) for bbox, score, class_name, feature in zip(bboxes, scores, names, features)]

    #initialize color map
    cmap = plt.get_cmap('tab20b')
    colors = [cmap(i)[:3] for i in np.linspace(0, 1, 20)]

    # run non-maxima supression
    boxs = np.array([d.tlwh for d in detections])
    scores = np.array([d.confidence for d in detections])
    classes = np.array([d.class_name for d in detections])
    indices = preprocessing.non_max_suppression(boxs, classes, nms_max_overlap, scores)
    detections = [detections[i] for i in indices]       

    # Call the tracker
    tracker.predict()
    tracker.update(detections)

    # update tracks
    for track in tracker.tracks:
        if not track.is_confirmed() or track.time_since_update > 1:
            continue 
        bbox = track.to_tlbr()
        class_name = track.get_class()
        
    # draw bbox on screen
        color = colors[int(track.track_id) % len(colors)]
        color = [i * 255 for i in color]
        cv2.rectangle(frame, (int(bbox[0]), int(bbox[1])), (int(bbox[2]), int(bbox[3])), color, 2)
        cv2.rectangle(frame, (int(bbox[0]), int(bbox[1]-30)), (int(bbox[0])+(len(class_name)+len(str(track.track_id)))*17, int(bbox[1])), color, -1)
        cv2.putText(frame, class_name + "-" + str(track.track_id),(int(bbox[0]), int(bbox[1]-10)),0, 0.75, (255,255,255),2)

    # if enable info flag then print details about each track
        if rospy.get_param('info'):
            print("Tracker ID: {}, Class: {},  BBox Coords (xmin, ymin, xmax, ymax): {}".format(str(track.track_id), class_name, (int(bbox[0]), int(bbox[1]), int(bbox[2]), int(bbox[3]))))
        if not found_target:
            target_ID = track.track_id
            x_pixel = int((bbox[0]+bbox[2])/2)
            y_pixel = int((bbox[1]+bbox[3])/2)
            print("Found Person to track (ID: {})".format(target_ID))
            print("Location of the person({}) : {} {}".format(track.track_id,x_pixel, y_pixel))
            found_target = True
        elif track.track_id == target_ID:
            x_pixel = int((bbox[0]+bbox[2])/2)
            y_pixel = int((bbox[1]+bbox[3])/2)
            print("Location of the person({}) : {} {}".format(track.track_id,x_pixel, y_pixel))
    # calculate frames per second of running detections
    fps = 1.0 / (time.time() - start_time)
    print("FPS: %.2f" % fps)
    result = np.asarray(frame)
    # result = cv2.cvtColor(frame, cv2.COLOR_RGB2BGR)
    
    if not rospy.get_param('dont_show'):
        cv2.imshow("Output Video", result)
    
    # if output flag is set, save video file
    # if FLAGS.output:
    #     out.write(result)
    if cv2.waitKey(1) & 0xFF == ord('q'): return

#Convert x,y, z from pixels to mm
def depth_callback(frame):
    global x_pixel
    global y_pixel
    global intrinsics
    global x_coord
    global y_coord
    global z_coord
    if (x_pixel is None) or (y_pixel is None):
        return
    try:
        cv_image = bridge.imgmsg_to_cv2(frame, frame.encoding)
        # pick one pixel among all the pixels with the closest range:
        indices = np.array(np.where(cv_image == cv_image[cv_image > 0].min()))[:,0]
        pix = (x_pixel, y_pixel)
        pix = pix
        line = '\rDepth at pixel(%3d, %3d): %7.1f(mm).' % (pix[0], pix[1], cv_image[pix[1], pix[0]])
        # cv_image_array = np.array(cv_image,dtype=np.dtype('f8'))
        # cv_image_norm = cv2.normalize(cv_image_array,cv_image_array,0,1,cv2.NORM_MINMAX)
        # cv_image_norm = cv2.circle(cv_image_norm, (pix[1],pix[0]), radius=5,color = 255)
        # cv2.imshow("camera",cv_image_norm)
        # cv2.waitKey(1)
        if intrinsics:
            depth = cv_image[pix[1], pix[0]]
            result = rs2.rs2_deproject_pixel_to_point(intrinsics, [pix[0], pix[1]], depth)
            x = result[0]/1000
            y = result[2]/1000
            line += '  Coordinate: %8.2f %8.2f %8.2f.' % (result[0], result[1], result[2])
            if y>=0.5:
                x_coord = x
                y_coord = y
        # if (not self.pix_grade is None):
        #     line += ' Grade: %2d' % self.pix_grade
        line += '\r'
        print(line)

    except CvBridgeError as e:
        print(e)
        return
    except ValueError as e:
        return
    pass

#Get camera info to help convert pixels to m
def info_callback(cameraInfo):
    global intrinsics
    try:
        if intrinsics:
            return
        intrinsics = rs2.intrinsics()
        intrinsics.width = cameraInfo.width
        intrinsics.height = cameraInfo.height
        intrinsics.ppx = cameraInfo.K[2]
        intrinsics.ppy = cameraInfo.K[5]
        intrinsics.fx = cameraInfo.K[0]
        intrinsics.fy = cameraInfo.K[4]
        if cameraInfo.distortion_model == 'plumb_bob':
            intrinsics.model = rs2.distortion.brown_conrady
        elif cameraInfo.distortion_model == 'equidistant':
            intrinsics.model = rs2.distortion.kannala_brandt4
        intrinsics.coeffs = [i for i in cameraInfo.D]
    except CvBridgeError as e:
        print(e)
        return
    pass

'''def main(_argv):
    # Definition of the parameters
    max_cosine_distance = 0.4
    nn_budget = None
    nms_max_overlap = 1.0
    
    # initialize deep sort
    model_filename = 'model_data/mars-small128.pb'
    encoder = gdet.create_box_encoder(model_filename, batch_size=1)
    # calculate cosine distance metric
    metric = nn_matching.NearestNeighborDistanceMetric("cosine", max_cosine_distance, nn_budget)
    # initialize tracker
    tracker = Tracker(metric)

    # load configuration for object detector
    config = ConfigProto()
    config.gpu_options.allow_growth = True
    session = InteractiveSession(config=config)
    STRIDES, ANCHORS, NUM_CLASS, XYSCALE = utils.load_config(FLAGS)
    input_size = rospy.get_param('size')
    video_path = rospy.get_param('video')

    # load tflite model if flag is set
    if rospy.get_param('framework') == 'tflite':
        interpreter = tf.lite.Interpreter(model_path=rospy.get_param('weights'))
        interpreter.allocate_tensors()
        input_details = interpreter.get_input_details()
        output_details = interpreter.get_output_details()
        print(input_details)
        print(output_details)
    # otherwise load standard tensorflow saved model
    else:
        saved_model_loaded = tf.saved_model.load(rospy.get_param('weights'), tags=[tag_constants.SERVING])
        infer = saved_model_loaded.signatures['serving_default']

    # begin video capture
    try:
        vid = cv2.VideoCapture(int(video_path))
    except:
        vid = cv2.VideoCapture(video_path)

    out = None

    # get video ready to save locally if flag is set
    if FLAGS.output: 
        # by default VideoCapture returns float instead of int
        width = int(vid.get(cv2.CAP_PROP_FRAME_WIDTH))
        height = int(vid.get(cv2.CAP_PROP_FRAME_HEIGHT))
        fps = int(vid.get(cv2.CAP_PROP_FPS))
        codec = cv2.VideoWriter_fourcc(*FLAGS.output_format)
        out = cv2.VideoWriter(FLAGS.output, codec, fps, (width, height))

    frame_num = 0
    # while video is running'''

if __name__ == '__main__':

    max_cosine_distance = 0.4
    nn_budget = None
    nms_max_overlap = 1.0

      # initialize deep sort
    model_filename = 'model_data/mars-small128.pb'
    encoder = gdet.create_box_encoder(model_filename, batch_size=1)
    # calculate cosine distance metric
    metric = nn_matching.NearestNeighborDistanceMetric("cosine", max_cosine_distance, nn_budget)
    # initialize tracker
    tracker = Tracker(metric)

    # load configuration for object detector
    config = ConfigProto()
    config.gpu_options.allow_growth = True
    session = InteractiveSession(config=config)
    # STRIDES, ANCHORS, NUM_CLASS, XYSCALE = utils.load_config(FLAGS)
    input_size = rospy.get_param('size')
    video_path = rospy.get_param('video')

    # load tflite model if flag is set
    if rospy.get_param('framework') == 'tflite':
        interpreter = tf.lite.Interpreter(model_path=rospy.get_param('weights'))
        interpreter.allocate_tensors()
        input_details = interpreter.get_input_details()
        output_details = interpreter.get_output_details()
        print(input_details)
        print(output_details)
    # otherwise load standard tensorflow saved model
    else:
        saved_model_loaded = tf.saved_model.load(rospy.get_param('weights'), tags=[tag_constants.SERVING])
        infer = saved_model_loaded.signatures['serving_default']

    # begin video capture
    # try:
    #     vid = cv2.VideoCapture(int(video_path))
    # except:
    #     vid = cv2.VideoCapture(video_path)
        
    
    # while video is running
    global frame_num
    frame_num = 0
    rospy.init_node('object_tracker')
    bridge = CvBridge()
    image_sub = rospy.Subscriber("/camera/color/image_raw", Image , yolo_callback)
    depth_sub = rospy.Subscriber("/camera/depth/image_rect_raw", Image , depth_callback)
    depth_info_sub = rospy.Subscriber("/camera/depth/camera_info", CameraInfo , info_callback)
    try:
        rospy.spin()
    except rospy.ROSInterruptException:
        rospy.loginfo("shutdown")
    cv2.destroyAllWindows()
    
    # try:
    #     app.run(main)
    # except SystemExit:
    #     pass
