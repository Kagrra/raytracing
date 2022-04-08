cmake_minimum_required(VERSION 3.22)

project(rt
  LANGUAGES CXX
  VERSION 0.0.1
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_executable(rt app/main.cpp)
target_include_directories(rt PRIVATE src app)

IF(MSVC)
    set_target_properties(rt PROPERTIES LINK_FLAGS /STACK:"10000000")
ENDIF(MSVC)
