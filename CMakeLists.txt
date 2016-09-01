cmake_minimum_required( VERSION 3.5 )
project( ZMQ_GPB_Example )

# set flags and module path
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Werror -Wextra -Wshadow" )
set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules/" )

# find packages
find_package( ZMQ REQUIRED )

# add all subdirectories
add_subdirectory( protobuf/ )
add_subdirectory( lib/ )
add_subdirectory( manager/ )
add_subdirectory( error/ )
add_subdirectory( monitor/ )

# set variable for all libraries
set( LIBS ${LIBS} main_lib gpb_lib ${ZMQ_LIBRARIES} )

# create the subscriber executable
add_executable( subscriber subscriber.cpp )
target_link_libraries( subscriber ${LIBS} )

# create the publisher executable
add_executable( publisher publisher.cpp )
target_link_libraries( publisher ${LIBS} )


