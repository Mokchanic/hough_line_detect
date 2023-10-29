cmake_minimum_required(VERSION 3.10)
project(line_detect_test/build)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
add_executable(line_detect_test src/opencv_test.cpp)
target_link_libraries(line_detect_test ${OpenCV_LIBS})