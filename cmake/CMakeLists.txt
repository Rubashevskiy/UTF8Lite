cmake_minimum_required(VERSION 3.0)
project(UTF8Lite)
set(CMAKE_CXX_STANDARD 11)
set(SRC "../example/main.cpp")
add_executable(${PROJECT_NAME} ${SRC})
add_definitions(-g)

