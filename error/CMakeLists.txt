# create error executable
add_executable( error main.cpp )
target_link_libraries( error
        gpb_lib
        main_lib
        ${ZMQ_LIBRARIES} )