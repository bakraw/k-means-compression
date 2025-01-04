cmake_minimum_required(VERSION 3.30)
project(k_means_compression)

set(CMAKE_CXX_STANDARD 26)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(k_means_compression main.cpp)
target_link_libraries(k_means_compression PUBLIC ${OpenCV_LIBS})