cmake_minimum_required(VERSION 3.28.0)
project(polynom-prac VERSION 0.1.0 LANGUAGES C CXX)
# set(CMAKE_CXX_VERSION 20)
set (CMAKE_CXX_STANDARD 20)

add_executable(polynom-prac 
    src/main.cpp
    src/engine/
)

include_directories(./src/engine ./src/engine/src)

# add_executable(polynom-prac ${polynom_SRC})

include_directories(./cpp-httplib/)
target_link_libraries(polynom-prac jsoncpp)