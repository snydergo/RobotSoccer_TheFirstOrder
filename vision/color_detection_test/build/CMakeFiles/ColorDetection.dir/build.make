# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

# Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/ecestudent/workspace/RobotSoccer_TheFirstOrder/vision/color_detection_test

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ecestudent/workspace/RobotSoccer_TheFirstOrder/vision/color_detection_test/build

# Include any dependencies generated for this target.
include CMakeFiles/ColorDetection.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/ColorDetection.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ColorDetection.dir/flags.make

CMakeFiles/ColorDetection.dir/colordetection.cpp.o: CMakeFiles/ColorDetection.dir/flags.make
CMakeFiles/ColorDetection.dir/colordetection.cpp.o: ../colordetection.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/ecestudent/workspace/RobotSoccer_TheFirstOrder/vision/color_detection_test/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/ColorDetection.dir/colordetection.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/ColorDetection.dir/colordetection.cpp.o -c /home/ecestudent/workspace/RobotSoccer_TheFirstOrder/vision/color_detection_test/colordetection.cpp

CMakeFiles/ColorDetection.dir/colordetection.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ColorDetection.dir/colordetection.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/ecestudent/workspace/RobotSoccer_TheFirstOrder/vision/color_detection_test/colordetection.cpp > CMakeFiles/ColorDetection.dir/colordetection.cpp.i

CMakeFiles/ColorDetection.dir/colordetection.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ColorDetection.dir/colordetection.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/ecestudent/workspace/RobotSoccer_TheFirstOrder/vision/color_detection_test/colordetection.cpp -o CMakeFiles/ColorDetection.dir/colordetection.cpp.s

CMakeFiles/ColorDetection.dir/colordetection.cpp.o.requires:
.PHONY : CMakeFiles/ColorDetection.dir/colordetection.cpp.o.requires

CMakeFiles/ColorDetection.dir/colordetection.cpp.o.provides: CMakeFiles/ColorDetection.dir/colordetection.cpp.o.requires
	$(MAKE) -f CMakeFiles/ColorDetection.dir/build.make CMakeFiles/ColorDetection.dir/colordetection.cpp.o.provides.build
.PHONY : CMakeFiles/ColorDetection.dir/colordetection.cpp.o.provides

CMakeFiles/ColorDetection.dir/colordetection.cpp.o.provides.build: CMakeFiles/ColorDetection.dir/colordetection.cpp.o

# Object files for target ColorDetection
ColorDetection_OBJECTS = \
"CMakeFiles/ColorDetection.dir/colordetection.cpp.o"

# External object files for target ColorDetection
ColorDetection_EXTERNAL_OBJECTS =

ColorDetection: CMakeFiles/ColorDetection.dir/colordetection.cpp.o
ColorDetection: CMakeFiles/ColorDetection.dir/build.make
ColorDetection: /usr/lib/x86_64-linux-gnu/libopencv_videostab.so.2.4.8
ColorDetection: /usr/lib/x86_64-linux-gnu/libopencv_video.so.2.4.8
ColorDetection: /usr/lib/x86_64-linux-gnu/libopencv_ts.so.2.4.8
ColorDetection: /usr/lib/x86_64-linux-gnu/libopencv_superres.so.2.4.8
ColorDetection: /usr/lib/x86_64-linux-gnu/libopencv_stitching.so.2.4.8
ColorDetection: /usr/lib/x86_64-linux-gnu/libopencv_photo.so.2.4.8
ColorDetection: /usr/lib/x86_64-linux-gnu/libopencv_ocl.so.2.4.8
ColorDetection: /usr/lib/x86_64-linux-gnu/libopencv_objdetect.so.2.4.8
ColorDetection: /usr/lib/x86_64-linux-gnu/libopencv_ml.so.2.4.8
ColorDetection: /usr/lib/x86_64-linux-gnu/libopencv_legacy.so.2.4.8
ColorDetection: /usr/lib/x86_64-linux-gnu/libopencv_imgproc.so.2.4.8
ColorDetection: /usr/lib/x86_64-linux-gnu/libopencv_highgui.so.2.4.8
ColorDetection: /usr/lib/x86_64-linux-gnu/libopencv_gpu.so.2.4.8
ColorDetection: /usr/lib/x86_64-linux-gnu/libopencv_flann.so.2.4.8
ColorDetection: /usr/lib/x86_64-linux-gnu/libopencv_features2d.so.2.4.8
ColorDetection: /usr/lib/x86_64-linux-gnu/libopencv_core.so.2.4.8
ColorDetection: /usr/lib/x86_64-linux-gnu/libopencv_contrib.so.2.4.8
ColorDetection: /usr/lib/x86_64-linux-gnu/libopencv_calib3d.so.2.4.8
ColorDetection: /usr/lib/x86_64-linux-gnu/libopencv_photo.so.2.4.8
ColorDetection: /usr/lib/x86_64-linux-gnu/libopencv_legacy.so.2.4.8
ColorDetection: /usr/lib/x86_64-linux-gnu/libopencv_video.so.2.4.8
ColorDetection: /usr/lib/x86_64-linux-gnu/libopencv_objdetect.so.2.4.8
ColorDetection: /usr/lib/x86_64-linux-gnu/libopencv_ml.so.2.4.8
ColorDetection: /usr/lib/x86_64-linux-gnu/libopencv_calib3d.so.2.4.8
ColorDetection: /usr/lib/x86_64-linux-gnu/libopencv_features2d.so.2.4.8
ColorDetection: /usr/lib/x86_64-linux-gnu/libopencv_highgui.so.2.4.8
ColorDetection: /usr/lib/x86_64-linux-gnu/libopencv_imgproc.so.2.4.8
ColorDetection: /usr/lib/x86_64-linux-gnu/libopencv_flann.so.2.4.8
ColorDetection: /usr/lib/x86_64-linux-gnu/libopencv_core.so.2.4.8
ColorDetection: CMakeFiles/ColorDetection.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable ColorDetection"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ColorDetection.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/ColorDetection.dir/build: ColorDetection
.PHONY : CMakeFiles/ColorDetection.dir/build

CMakeFiles/ColorDetection.dir/requires: CMakeFiles/ColorDetection.dir/colordetection.cpp.o.requires
.PHONY : CMakeFiles/ColorDetection.dir/requires

CMakeFiles/ColorDetection.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ColorDetection.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ColorDetection.dir/clean

CMakeFiles/ColorDetection.dir/depend:
	cd /home/ecestudent/workspace/RobotSoccer_TheFirstOrder/vision/color_detection_test/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ecestudent/workspace/RobotSoccer_TheFirstOrder/vision/color_detection_test /home/ecestudent/workspace/RobotSoccer_TheFirstOrder/vision/color_detection_test /home/ecestudent/workspace/RobotSoccer_TheFirstOrder/vision/color_detection_test/build /home/ecestudent/workspace/RobotSoccer_TheFirstOrder/vision/color_detection_test/build /home/ecestudent/workspace/RobotSoccer_TheFirstOrder/vision/color_detection_test/build/CMakeFiles/ColorDetection.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/ColorDetection.dir/depend

