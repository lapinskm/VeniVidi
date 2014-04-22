cmake_minimum_required(VERSION 2.8)

project( VeniVidi )

find_package( OpenCV REQUIRED )

include_directories(include)
set (CMAKE_CXX_FLAGS "-std=c++11")

#LIBRARY
file(GLOB LIB_SOURCE lib_src/*)

add_library( VeniVidi ${LIB_SOURCE} )
target_link_libraries( VeniVidi ${OpenCV_LIBS} )

#EXECUTABLE
file(GLOB APP_SOURCE app_src/*)

add_executable( VeniVidiVici  ${APP_SOURCE} )
target_link_libraries(VeniVidiVici VeniVidi)

#UNITTESTS
add_subdirectory(gtest)
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

file(GLOB TEST_SOURCE test/*.cpp) 

add_executable( runUnitTests ${TEST_SOURCE} )
target_link_libraries(runUnitTests gtest gtest_main  ${OpenCV_LIBS} VeniVidi)

configure_file(test/data/lena.png ${CMAKE_CURRENT_BINARY_DIR}/lena.png COPYONLY)
configure_file(test/data/lena-gray.png ${CMAKE_CURRENT_BINARY_DIR}/lena-gray.png COPYONLY)

add_test( runUnitTests runUnitTests )

