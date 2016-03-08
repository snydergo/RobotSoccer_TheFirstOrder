cmake_minimum_required(VERSION 2.8.3)
project(walle)

# Find catkin and any catkin packages
find_package(catkin REQUIRED COMPONENTS roscpp gazebo_ros rospy std_msgs geometry_msgs message_generation cv_bridge genmsg image_transport sensor_msgs xacro)

# Depend on system install of Gazebo
find_package(gazebo REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GAZEBO_CXX_FLAGS}")
link_directories(${GAZEBO_LIBRARY_DIRS})

# Declare ROS messages and services
add_message_files(DIRECTORY msg FILES SoccerPoses.msg)

# Generate added messages and services
generate_messages(DEPENDENCIES std_msgs geometry_msgs)

# Declare a catkin package
catkin_package(CATKIN_DEPENDS message_runtime roscpp gazebo_ros)

# Specify additional locations of header files
include_directories(include ${Boost_INCLUDE_DIR} ${catkin_INCLUDE_DIRS} ${GAZEBO_INCLUDE_DIRS})

# Declare C++ executables
add_executable(vision src/vision.cpp)
find_package( OpenCV REQUIRED )
target_link_libraries(vision ${OpenCV_LIBS} ${catkin_LIBRARIES})

add_executable(controller src/controller.cpp)
target_link_libraries(controller ${catkin_LIBRARIES})

# Add plugins
add_library(soccer_drive plugins/soccer_drive.cpp)
target_link_libraries(soccer_drive ${catkin_LIBRARIES} ${GAZEBO_LIBRARIES})

add_library(soccer_ball plugins/soccer_ball.cpp)
target_link_libraries(soccer_ball ${catkin_LIBRARIES} ${GAZEBO_LIBRARIES})

# Generate .world files from .world.xacro files

# Xacro files
file(GLOB xacro_files ${CMAKE_CURRENT_SOURCE_DIR}/worlds/*.world.xacro)

foreach(it ${xacro_files})
  # remove .xacro extension
  string(REGEX MATCH "(.*)[.]xacro$" unused ${it})
  set(output_filename ${CMAKE_MATCH_1})

  # create a rule to generate ${output_filename} from {it}
  xacro_add_xacro_file(${it} ${output_filename})

  list(APPEND world_files ${output_filename})
endforeach(it)

# add an abstract target to actually trigger the builds
add_custom_target(media_files ALL DEPENDS ${world_files})
