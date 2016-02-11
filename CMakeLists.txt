cmake_minimum_required(VERSION 2.8.3)
project(robot_soccer)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  message_generation
)

catkin_python_setup()

add_message_files(
  FILES
  visiondata.msg
  controldata.msg
)

generate_messages(
  DEPENDENCIES
  std_msgs
)

catkin_package(
  CATKIN_DEPENDS message_runtime rospy
)

include_directories(
  ${catkin_INCLUDE_DIRS}
)

catkin_install_python(PROGRAMS
AI/visiondata/subscriber_VisionMsg.py
AI/maincontrol/StarKillerMainControl.py
motion_control/library/subscriber_CtrlMsg.py
AI/maincontrol/publisher_CtrlMsg.py
DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})


add_definitions(-std=c++11)
find_package( OpenCV REQUIRED )

## Declare a C++ executable
add_executable( CalibrateField vision/calibrate_field/calibratefield.cpp )
target_link_libraries( CalibrateField ${OpenCV_LIBS} )

add_executable( CenterField vision/center_field/centerfield.cpp )
target_link_libraries( CenterField ${OpenCV_LIBS} )

add_executable( ColorDetection vision/color_detection_test/colordetection.cpp )
target_link_libraries( ColorDetection ${OpenCV_LIBS} )

add_executable( MultiobjectDetection vision/multiobject_detection_test/multiobjectdetection.cpp )
target_link_libraries( MultiobjectDetection ${OpenCV_LIBS} )

add_executable( ObjectTracking vision/object_tracking_test/objecttracking.cpp )
target_link_libraries( ObjectTracking ${OpenCV_LIBS} )

add_executable( RobotVision 
vision/robot_vision/main.cpp
vision/robot_vision/hsvcolorsubspace.h
vision/robot_vision/undefinedcvobject.h
vision/robot_vision/undefinedcvobject.cpp
vision/robot_vision/utils.h
vision/robot_vision/utils.cpp
vision/robot_vision/objectdetection.h
vision/robot_vision/objectdetection.cpp
vision/robot_vision/config.h
vision/robot_vision/config.cpp
)
target_link_libraries( RobotVision ${OpenCV_LIBS} )
target_link_libraries( RobotVision ${catkin_LIBRARIES})
include_directories(~/imports/rapidjson/include)


add_executable( MainControl 
AI_cpp/maincontrol.cpp
AI_cpp/visiondata/subscriber_visionmsg.h
AI_cpp/visiondata/subscriber_visionmsg.cpp
AI_cpp/ctrldata/publisher_ctrlmsg.h
AI_cpp/ctrldata/publisher_ctrlmsg.cpp
AI_cpp/bookkeeping.h
AI_cpp/bookkeeping.cpp
AI_cpp/exceptionAI.h
AI_cpp/exceptionAI.cpp
AI_cpp/mathfunctions.h
AI_cpp/mathfunctions.cpp
AI_cpp/dataclasses/gamestatus.h
AI_cpp/dataclasses/gamestatus.cpp
AI_cpp/dataclasses/point.h
AI_cpp/dataclasses/point.cpp
AI_cpp/dataclasses/robot.h
AI_cpp/dataclasses/robot.cpp
AI_cpp/dataclasses/fieldobject.h
AI_cpp/dataclasses/fieldobject.cpp
AI_cpp/gameplay/fieldcoord.h
AI_cpp/gameplay/fieldcoord.cpp
AI_cpp/gameplay/utilities.h
AI_cpp/gameplay/utilities.cpp
)
target_link_libraries( MainControl ${catkin_LIBRARIES})
