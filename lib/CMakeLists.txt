find_package( ZMQ REQUIRED )

# set the include directories
include_directories( include/ )                        # for main lib files

# set source files
set( SOURCES utilities.cpp )

# create main library
add_library( main_lib ${SOURCES} )
target_include_directories( main_lib PUBLIC include/)
target_link_libraries( main_lib PUBLIC gpb_lib )


