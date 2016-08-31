cmake_minimum_required( VERSION 3.5 )
project( ZMQ_GPB_Example )

# set flags and module path
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Werror -Wextra -Wshadow" )
set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules/" )

find_package( ZMQ REQUIRED )

add_subdirectory( protobuf/ )

# set the include directories
include_directories( ${CMAKE_CURRENT_BINARY_DIR} )     # for generated proto files
include_directories( include/ )                        # for include files
include_directories( ${ZMQ_INCLUDE_DIR} )              # for ZMQ files

# create the subsriber executable
add_executable( subscriber subscriber.cpp )
target_link_libraries( subscriber PUBLIC gpb_lib ${ZMQ_LIBRARIES} )

# create the spublisher executable
add_executable( publisher publisher.cpp )
target_link_libraries( publisher PUBLIC gpb_lib ${ZMQ_LIBRARIES} )


