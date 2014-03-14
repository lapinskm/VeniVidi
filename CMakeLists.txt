cmake_minimum_required(VERSION 2.8)

project( VeniVidi )

find_package( OpenCV REQUIRED )

add_executable( VeniVidi src/VeniVidi.cpp )

target_link_libraries( VeniVidi ${OpenCV_LIBS} )
