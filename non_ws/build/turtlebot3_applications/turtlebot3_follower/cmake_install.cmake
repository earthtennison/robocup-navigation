# Install script for directory: /home/pirat/robocup-navigation/non_ws/src/turtlebot3_applications/turtlebot3_follower

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/pirat/robocup-navigation/non_ws/install")
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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "/home/pirat/robocup-navigation/non_ws/build/turtlebot3_applications/turtlebot3_follower/catkin_generated/installspace/turtlebot3_follower.pc")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/turtlebot3_follower/cmake" TYPE FILE FILES
    "/home/pirat/robocup-navigation/non_ws/build/turtlebot3_applications/turtlebot3_follower/catkin_generated/installspace/turtlebot3_followerConfig.cmake"
    "/home/pirat/robocup-navigation/non_ws/build/turtlebot3_applications/turtlebot3_follower/catkin_generated/installspace/turtlebot3_followerConfig-version.cmake"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/turtlebot3_follower" TYPE FILE FILES "/home/pirat/robocup-navigation/non_ws/src/turtlebot3_applications/turtlebot3_follower/package.xml")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/turtlebot3_follower" TYPE PROGRAM FILES
    "/home/pirat/robocup-navigation/non_ws/src/turtlebot3_applications/turtlebot3_follower/nodes/follower"
    "/home/pirat/robocup-navigation/non_ws/src/turtlebot3_applications/turtlebot3_follower/nodes/laser_subscriber"
    "/home/pirat/robocup-navigation/non_ws/src/turtlebot3_applications/turtlebot3_follower/nodes/ploting"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/turtlebot3_follower" TYPE DIRECTORY FILES "/home/pirat/robocup-navigation/non_ws/src/turtlebot3_applications/turtlebot3_follower/rviz")
endif()

