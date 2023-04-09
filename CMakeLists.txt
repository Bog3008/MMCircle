cmake_minimum_required(VERSION 3.21)
project(MMCircle)

set(CMAKE_CXX_STANDARD 23)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
set(OpenCV_LIBS opencv_core opencv_imgproc opencv_highgui opencv_imgcodecs)

add_executable(MMCircle main.cpp)

target_link_libraries(MMCircle
        ${OpenCV_LIBS})