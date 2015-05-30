cmake_minimum_required( VERSION 2.8 )

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -Wall")

include_directories( .. )
link_directories( .. )
link_libraries( ${CMAKE_CURRENT_SOURCE_DIR}/../libjson.a )
add_executable( test_libjson test_libjson.cpp )

