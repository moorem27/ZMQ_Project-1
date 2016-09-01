# create monitor executable
add_executable( monitor main.cpp )
target_link_libraries( monitor
        gpb_lib
        main_lib
        ${ZMQ_LIBRARIES} )