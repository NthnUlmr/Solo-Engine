cmake_minimum_required(VERSION 3.4)
set(CMAKE_CXX_STANDARD 17)

project(SoloEngine VERSION 0.0.1 LANGUAGES CXX)


include(CTest)

add_subdirectory(src)
add_subdirectory(tests)
