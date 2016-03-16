cmake_minimum_required(VERSION 2.8.3)
project(robot_soccer)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  message_generation
)

#catkin_python_setup()

add_message_files(
  FILES
  visiondata.msg
  controldata.msg
  piddata.msg
  gameparam.msg
)

generate_messages(
  DEPENDENCIES
  std_msgs
)

catkin_package(
  CATKIN_DEPENDS message_runtime rospy
)

include_directories( include
  ${catkin_INCLUDE_DIRS}
)

catkin_install_python(PROGRAMS
motion_control/library/subscriber_CtrlMsg.py
motion_control/library/manual.py
ai/kalmanfilter/kfiltercom.py
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

add_executable( GameControl game_control/gamecontroller.cpp )
target_link_libraries( GameControl ${catkin_LIBRARIES})

add_executable( RobotVision 
vision/robot_vision/main.cpp
vision/robot_vision/hsvcolorsubspace.h
vision/robot_vision/undefinedcvobject.h
vision/robot_vision/undefinedcvobject.cpp
vision/robot_vision/definedcvobject.h
vision/robot_vision/robot.h
vision/robot_vision/robot.cpp
vision/robot_vision/parameters.h
vision/robot_vision/parameters.cpp
vision/robot_vision/ball.h
vision/robot_vision/ball.cpp
vision/robot_vision/utils.h
vision/robot_vision/objectdetection.h
vision/robot_vision/objectdetection.cpp
vision/robot_vision/config.h
vision/robot_vision/config.cpp
)
add_dependencies(RobotVision robot_soccer_generate_messages_cpp  ${catkin_EXPORTED_TARGETS})
target_link_libraries( RobotVision ${OpenCV_LIBS} )
target_link_libraries( RobotVision ${catkin_LIBRARIES})
#include_directories(../../../imports/rapidjson/include)

add_executable( debug
ai/debug/publisher_debugmsg.h
ai/debug/publisher_debugmsg.cpp
ai/visiondata/subscriber_visionmsg.h
ai/visiondata/subscriber_visionmsg.cpp
ai/ctrldata/publisher_ctrlmsg.h
ai/ctrldata/publisher_ctrlmsg.cpp
ai/bookkeeping.h
ai/bookkeeping.cpp
ai/types.h
ai/exceptionAI.h
ai/exceptionAI.cpp
ai/mathfunctions.h
ai/mathfunctions.cpp
ai/dataclasses/gamestatus.h
ai/dataclasses/gamestatus.cpp
ai/dataclasses/point.h
ai/dataclasses/point.cpp
ai/dataclasses/robot.h
ai/dataclasses/robot.cpp
ai/dataclasses/fieldobject.h
ai/dataclasses/fieldobject.cpp
ai/gameplay/fieldcoord.h
ai/gameplay/fieldcoord.cpp
ai/gameplay/utilities.h
ai/gameplay/utilities.cpp
ai/gameplay/skills.h
ai/gameplay/skills.cpp
ai/gameplay/strategy.h
ai/gameplay/strategy.cpp
ai/gameplay/plays.h
ai/gameplay/plays.cpp
)
add_dependencies(debug robot_soccer_generate_messages_cpp  ${catkin_EXPORTED_TARGETS})
target_link_libraries( debug ${catkin_LIBRARIES})

add_executable( MainControl 
ai/maincontrol.cpp
ai/visiondata/subscriber_visionmsg.h
ai/visiondata/subscriber_visionmsg.cpp
ai/ctrldata/publisher_ctrlmsg.h
ai/ctrldata/publisher_ctrlmsg.cpp
ai/debug/subscriber_debugmsg.cpp
ai/debug/subscriber_debugmsg.h
ai/kalmanfilter/getFilteredData.h
ai/kalmanfilter/getFilteredData.cpp
ai/bookkeeping.h
ai/bookkeeping.cpp
ai/types.h
ai/exceptionAI.h
ai/exceptionAI.cpp
ai/mathfunctions.h
ai/mathfunctions.cpp
ai/dataclasses/gamestatus.h
ai/dataclasses/gamestatus.cpp
ai/dataclasses/point.h
ai/dataclasses/point.cpp
ai/dataclasses/robot.h
ai/dataclasses/robot.cpp
ai/dataclasses/fieldobject.h
ai/dataclasses/fieldobject.cpp
ai/gameplay/fieldcoord.h
ai/gameplay/fieldcoord.cpp
ai/gameplay/utilities.h
ai/gameplay/utilities.cpp
ai/gameplay/skills.h
ai/gameplay/skills.cpp
ai/gameplay/strategy.h
ai/gameplay/strategy.cpp
ai/gameplay/plays.h
ai/gameplay/plays.cpp
)
add_dependencies(MainControl robot_soccer_generate_messages_cpp  ${catkin_EXPORTED_TARGETS})
target_link_libraries( MainControl ${catkin_LIBRARIES})
