cmake_minimum_required(VERSION 3.30)
project(k_means_compression)

set(CMAKE_CXX_STANDARD 26)

find_package(OpenCV REQUIRED)
find_package(OpenMP REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS} include)

add_executable(k_means_compression
                src/main.cpp
                include/Image.hpp
                src/Image.cpp)
target_link_libraries(k_means_compression PUBLIC ${OpenCV_LIBS} OpenMP::OpenMP_CXX)