cmake_minimum_required(VERSION 2.8)

project( VeniVidi )

find_package( OpenCV REQUIRED )

file(GLOB SOURCEFILES src/*)

include_directories(include)

set (CMAKE_CXX_FLAGS "-std=c++11")

add_executable( VeniVidi ${SOURCEFILES} )

target_link_libraries( VeniVidi ${OpenCV_LIBS} )

#UNITTESTS
add_subdirectory(gtest)
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
file(GLOB TESTFILES test/*.cpp) 

add_executable( runUnitTests ${SOURCEFILES} ${TESTFILES} )

# Link test executable against gtest & gtest_main
target_link_libraries(runUnitTests gtest gtest_main  ${OpenCV_LIBS})
add_test( runUnitTests runUnitTests )

