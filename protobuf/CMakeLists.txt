cmake_minimum_required( VERSION 3.5 )
project( Test_GPB  )

find_package( Protobuf REQUIRED )

# set includes and source files
include_directories( ${PROTOBUF_INCLUDE_DIRS} )
include_directories( ${CMAKE_CURRENT_BINARY_DIR} )
set( SOURCES messages.proto )

protobuf_generate_cpp( PROTO_SRCS PROTO_HDRS ${SOURCES} )

# create gpb library
add_library( gpb_lib ${PROTO_SRCS} ${PROTO_HDRS} )
target_include_directories( gpb_lib PUBLIC ${PROTOBUF_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR} )
target_link_libraries( gpb_lib PUBLIC ${PROTOBUF_LIBRARIES} )

