include_directories( ${CMAKE_BINARY_DIR} )

# create manager executable
add_executable( manager main.cpp )
target_link_libraries( manager
        gpb_lib
        main_lib
        ${ZMQ_LIBRARIES} )