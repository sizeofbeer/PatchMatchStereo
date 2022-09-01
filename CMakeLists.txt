cmake_minimum_required(VERSION 3.11)

PROJECT(PatchMatchStereo
	VERSION 0.1
	LANGUAGES C CXX
)

###############################################################################
# Build type and C++ compiler setup
###############################################################################

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)

###############################################################################
# Dependencies (ordered alphabetically)
###############################################################################

#############
#  OPENCV4  #
#############

find_package(OpenCV REQUIRED)
include_directories(${OPENCV_INCLUDE_DIRS})

###############################################################################
# Program
###############################################################################

set(SOURCES
	PatchMatchStereo/PatchMatchStereo.cpp
	PatchMatchStereo/pms_propagation.cpp
	PatchMatchStereo/pms_util.cpp
	PatchMatchStereo/stdafx.cpp
)

include_directories("PatchMatchStereo")
add_library(Stereo STATIC ${SOURCES})
target_link_libraries(Stereo PUBLIC ${OpenCV_LIBS} -pthread)

add_definitions(-w)
add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PUBLIC Stereo)
