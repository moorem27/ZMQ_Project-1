cmake_minimum_required( VERSION 3.5 )
project( Test_GPB  )

find_package( Protobuf REQUIRED )

set( SOURCES messages.proto )

protobuf_generate_cpp( PROTO_SRCS PROTO_HDRS ${SOURCES} )

# create gpb library
add_library( gpb_lib ${PROTO_SRCS} ${PROTO_HDRS} )
target_include_directories( gpb_lib PUBLIC ${PROBUF_INCLUDE_DIRS} ${CMAKE_BINARY_DIR} )
target_link_libraries( gpb_lib PUBLIC ${PROTOBUF_LIBRARIES} )

