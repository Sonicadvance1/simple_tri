cmake_minimum_required(VERSION 3.2)

set(PROJECT compute_test)
add_definitions(-std=c++1y)

include(FindOpenGL)
include_directories(${OPENGL_INCLUDE_DIR})

find_library(EPOXY_LIBRARY epoxy)
find_library(WAFFLE_LIBRARY waffle)

set(SRC Main.cpp
        Context.cpp)
set(LIBS epoxy waffle-1)

add_executable(${PROJECT} ${SRC})
target_link_libraries(${PROJECT} ${LIBS})
