cmake_minimum_required(VERSION 3.0.2)
project(multi_agent_sac)

find_package(catkin REQUIRED COMPONENTS
  geometry_msgs
  roslib
  rospy
  std_msgs
)

catkin_python_setup()

catkin_package(
 CATKIN_DEPENDS geometry_msgs roslib rospy std_msgs
)
