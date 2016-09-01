# set variable for all libraries
set( LIBS ${LIBS} main_lib gpb_lib ${ZMQ_LIBRARIES} )

# create the broker executable
add_executable( broker broker.cpp )
target_link_libraries( broker ${LIBS} )

# create the client executable
add_executable( client client.cpp )
target_link_libraries( client ${LIBS} )

# create the worker executable
add_executable( worker worker.cpp )
target_link_libraries( worker ${LIBS} )


