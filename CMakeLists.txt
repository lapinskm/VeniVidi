cmake_minimum_required(VERSION 2.8)

project( VeniVidi )

find_package( OpenCV REQUIRED )

file(GLOB SOURCEFILES src/*)

include_directories(include)

set (CMAKE_CXX_FLAGS "-std=c++11")

add_executable( VeniVidi ${SOURCEFILES} )

target_link_libraries( VeniVidi ${OpenCV_LIBS} )
