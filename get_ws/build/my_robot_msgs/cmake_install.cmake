# Install script for directory: /home/get/robocup-navigation/catkin_ws/src/my_robot_msgs

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/get/robocup-navigation/catkin_ws/install")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/my_robot_msgs/msg" TYPE FILE FILES "/home/get/robocup-navigation/catkin_ws/src/my_robot_msgs/msg/HardwareStatus.msg")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/my_robot_msgs/srv" TYPE FILE FILES
    "/home/get/robocup-navigation/catkin_ws/src/my_robot_msgs/srv/ComputeDiskArea.srv"
    "/home/get/robocup-navigation/catkin_ws/src/my_robot_msgs/srv/SetLed.srv"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/my_robot_msgs/action" TYPE FILE FILES
    "/home/get/robocup-navigation/catkin_ws/src/my_robot_msgs/action/CountUntil.action"
    "/home/get/robocup-navigation/catkin_ws/src/my_robot_msgs/action/MoveRobot.action"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/my_robot_msgs/msg" TYPE FILE FILES
    "/home/get/robocup-navigation/catkin_ws/devel/share/my_robot_msgs/msg/CountUntilAction.msg"
    "/home/get/robocup-navigation/catkin_ws/devel/share/my_robot_msgs/msg/CountUntilActionGoal.msg"
    "/home/get/robocup-navigation/catkin_ws/devel/share/my_robot_msgs/msg/CountUntilActionResult.msg"
    "/home/get/robocup-navigation/catkin_ws/devel/share/my_robot_msgs/msg/CountUntilActionFeedback.msg"
    "/home/get/robocup-navigation/catkin_ws/devel/share/my_robot_msgs/msg/CountUntilGoal.msg"
    "/home/get/robocup-navigation/catkin_ws/devel/share/my_robot_msgs/msg/CountUntilResult.msg"
    "/home/get/robocup-navigation/catkin_ws/devel/share/my_robot_msgs/msg/CountUntilFeedback.msg"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/my_robot_msgs/msg" TYPE FILE FILES
    "/home/get/robocup-navigation/catkin_ws/devel/share/my_robot_msgs/msg/MoveRobotAction.msg"
    "/home/get/robocup-navigation/catkin_ws/devel/share/my_robot_msgs/msg/MoveRobotActionGoal.msg"
    "/home/get/robocup-navigation/catkin_ws/devel/share/my_robot_msgs/msg/MoveRobotActionResult.msg"
    "/home/get/robocup-navigation/catkin_ws/devel/share/my_robot_msgs/msg/MoveRobotActionFeedback.msg"
    "/home/get/robocup-navigation/catkin_ws/devel/share/my_robot_msgs/msg/MoveRobotGoal.msg"
    "/home/get/robocup-navigation/catkin_ws/devel/share/my_robot_msgs/msg/MoveRobotResult.msg"
    "/home/get/robocup-navigation/catkin_ws/devel/share/my_robot_msgs/msg/MoveRobotFeedback.msg"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/my_robot_msgs/cmake" TYPE FILE FILES "/home/get/robocup-navigation/catkin_ws/build/my_robot_msgs/catkin_generated/installspace/my_robot_msgs-msg-paths.cmake")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "/home/get/robocup-navigation/catkin_ws/devel/include/my_robot_msgs")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/roseus/ros" TYPE DIRECTORY FILES "/home/get/robocup-navigation/catkin_ws/devel/share/roseus/ros/my_robot_msgs")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/common-lisp/ros" TYPE DIRECTORY FILES "/home/get/robocup-navigation/catkin_ws/devel/share/common-lisp/ros/my_robot_msgs")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/gennodejs/ros" TYPE DIRECTORY FILES "/home/get/robocup-navigation/catkin_ws/devel/share/gennodejs/ros/my_robot_msgs")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  execute_process(COMMAND "/usr/bin/python2" -m compileall "/home/get/robocup-navigation/catkin_ws/devel/lib/python2.7/dist-packages/my_robot_msgs")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/python2.7/dist-packages" TYPE DIRECTORY FILES "/home/get/robocup-navigation/catkin_ws/devel/lib/python2.7/dist-packages/my_robot_msgs")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "/home/get/robocup-navigation/catkin_ws/build/my_robot_msgs/catkin_generated/installspace/my_robot_msgs.pc")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/my_robot_msgs/cmake" TYPE FILE FILES "/home/get/robocup-navigation/catkin_ws/build/my_robot_msgs/catkin_generated/installspace/my_robot_msgs-msg-extras.cmake")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/my_robot_msgs/cmake" TYPE FILE FILES
    "/home/get/robocup-navigation/catkin_ws/build/my_robot_msgs/catkin_generated/installspace/my_robot_msgsConfig.cmake"
    "/home/get/robocup-navigation/catkin_ws/build/my_robot_msgs/catkin_generated/installspace/my_robot_msgsConfig-version.cmake"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/my_robot_msgs" TYPE FILE FILES "/home/get/robocup-navigation/catkin_ws/src/my_robot_msgs/package.xml")
endif()

