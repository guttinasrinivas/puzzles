add_executable(remove_nth_node_c 
               C/main.c
               C/src/remove_nth_node.c
               $ENV{VAS_LIB_HOME}/src/ref_ctd.c)

add_executable(remove_nth_node_cpp
               CPP/main.cpp
               $ENV{VAS_LIB_HOME}/src/SimpleLogger.cpp)

foreach (PROJ_NAME IN ITEMS remove_nth_node_c remove_nth_node_cpp)
    message(STATUS "Setting options for ${PROJ_NAME}")
    target_include_directories(${PROJ_NAME} PRIVATE $ENV{VAS_LIB_HOME}/inc)
    target_link_directories(${PROJ_NAME} PUBLIC $ENV{VAS_LIB_HOME}/build)
    if(NOT APPLE)
        target_link_options(${PROJ_NAME} PRIVATE -static)
    endif()
endforeach()

target_include_directories(remove_nth_node_c PRIVATE C/inc)
target_link_libraries(remove_nth_node_c PRIVATE wl_impl c_queue_impl)

target_include_directories(remove_nth_node_cpp PRIVATE CPP/inc)

