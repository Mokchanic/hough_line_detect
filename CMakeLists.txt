cmake_minimum_required(VERSION 3.10)

# Excutable project

project(
	LINE_DETECT_TEST
	LANGUAGES CXX
	VERSION 0.1.0
	DESCRIPTION "Line detect project"
)

set(${PROJECT_NAME}_PATH ${CMAKE_CURRENT_SOURCE_DIR})
set(${PROJECT_NAME}_INCLUDE_PATH ${${PROJECT_NAME}_PATH}/include)
set(${PROJECT_NAME}_RESOURCE_PATH ${${PROJECT_NAME}_PATH}/resource)
set(${PROJECT_NAME}_SOURCE_PATH ${${PROJECT_NAME}_PATH}/src)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_VERBOSE_MAKEFILE ON)

message(STATUS "----------------------------------------------")
message(STATUS "PROJECT_NAME: ${PROJECT_NAME}")
message(STATUS "${PROJECT_NAME}_VERSION: ${PROJECT_VERSION}")
message(STATUS "${PROJECT_NAME}_DESCRIPTION: ${PROJECT_DESCRIPTION}")
message(STATUS " ")
message(STATUS "${PROJECT_NAME}_PATH: ${${PROJECT_NAME}_PATH}")
message(STATUS "${PROJECT_NAME}_INCLUDE_PATH: ${${PROJECT_NAME}_INCLUDE_PATH}")
message(STATUS "${PROJECT_NAME}_RESOURCE_PATH: ${${PROJECT_NAME}_RESOURCE_PATH}")
message(STATUS "${PROJECT_NAME}_SOURCE_PATH: ${${PROJECT_NAME}_SOURCE_PATH}")
message(STATUS "----------------------------------------------")

add_subdirectory(sub_project)

add_executable(${PROJECT_NAME}
	${${PROJECT_NAME}_SOURCE_PATH}/main.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
	${${PROJECT_NAME}_INCLUDE_PATH}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
	#SUB_PROJECT
)

target_compile_options(${PROJECT_NAME} PRIVATE
	-Wall -Wextra -Wpedantic -Werror
)

# find_package(OpenCV REQUIRED)
# include_directories(${OpenCV_INCLUDE_DIRS})
# add_executable(line_detect_test src/opencv_test.cpp)
# target_link_libraries(line_detect_test ${OpenCV_LIBS})
