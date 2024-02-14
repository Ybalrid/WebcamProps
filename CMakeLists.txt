cmake_minimum_required(VERSION 3.26)
project(webcamprops)
set(CMAKE_CXX_STANDARD 23)

add_subdirectory(libdshowcapture)

add_executable(webcamprops WIN32 main.cpp)
target_link_libraries(webcamprops libdshowcapture)
