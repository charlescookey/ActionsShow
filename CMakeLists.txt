cmake_minimum_required(VERSION 3.5)

project(opencv-test-project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

FIND_PACKAGE( OpenCV REQUIRED )
INCLUDE_DIRECTORIES( ${OpenCV_INCLUDE_DIRS} )

add_executable(opencv-test-project "main.cpp" "ezOptionParser.hpp" "TargetOrientation.h" "TargetOrientation.cpp")
TARGET_LINK_LIBRARIES (opencv-test-project ${OpenCV_LIBS})

enable_testing()

add_executable(my_test tests.cpp)
add_test(Test1 my_test)
