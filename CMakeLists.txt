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

#catkin_install_python(PROGRAMS
#bin/hello
#listener.py
#AI/StarKillerMainControl.py
#  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})

catkin_install_python(PROGRAMS
AI/visiondata/listener.py
AI/main_control/StarKillerMainControl.py
DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})


#AI/visiondata/__init__
#AI/visiondata/GameStatus
#AI/visiondata/RosDataImporter
#AI/Gameplay/__init__
#AI/Gameplay/plays
#AI/Gameplay/skills
#AI/Gameplay/strategies
#AI/Gameplay/utilities
#AI/__init__
#AI/dataClasses
#AI/MathFunctions

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

add_executable( Talker vision/talker_test/talker.cpp )
target_link_libraries( Talker ${catkin_LIBRARIES})
target_link_libraries( Talker ${OpenCV_LIBS} )

add_executable( RobotVision 
vision/robot_vision/main.cpp
vision/robot_vision/hsvcolorsubspace.h
vision/robot_vision/undefinedcvobject.h
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
