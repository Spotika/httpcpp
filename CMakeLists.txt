cmake_minimum_required(VERSION 3.28.0)
project(polynom-prac VERSION 0.1.0 LANGUAGES C CXX)

add_executable(polynom-prac 
    src/main.cpp
)

include_directories(./cpp-httplib/)
target_link_libraries(polynom-prac jsoncpp)