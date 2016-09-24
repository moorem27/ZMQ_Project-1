# create error executable
add_executable( server main.cpp game_server.cc snake.cc )

include_directories("${CMAKE_SOURCE_DIR}/server/include")

target_link_libraries( server
        gpb_lib
        main_lib
        ${ZMQ_LIBRARIES} )
