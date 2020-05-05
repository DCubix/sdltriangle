cmake_minimum_required(VERSION 3.14)
project(sdltriangle)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-DSDL_MAIN_HANDLED)

file(GLOB SRC "src/*.h" "src/*.cpp")

find_package(SDL2 CONFIG REQUIRED)
add_executable(${PROJECT_NAME} ${SRC})
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2)