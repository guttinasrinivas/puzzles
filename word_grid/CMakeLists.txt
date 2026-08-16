add_executable(word_grid_c 
               C/main.c
               C/src/word_grid.c
               $ENV{VAS_LIB_HOME}/src/ref_ctd.c)

add_executable(word_grid_cpp
               CPP/main.cpp
               CPP/src/word_grid.cpp
               $ENV{VAS_LIB_HOME}/src/SimpleLogger.cpp)

foreach (PROJ_NAME IN ITEMS word_grid_c word_grid_cpp)
    message(STATUS "Setting options for ${PROJ_NAME}")
    target_include_directories(${PROJ_NAME} PRIVATE $ENV{VAS_LIB_HOME}/inc)
    target_link_directories(${PROJ_NAME} PUBLIC $ENV{VAS_LIB_HOME}/build)
    if(NOT APPLE)
        target_link_options(${PROJ_NAME} PRIVATE -static)
    endif()
endforeach()

target_include_directories(word_grid_c PRIVATE C/inc)
target_link_libraries(word_grid_c PRIVATE wl_impl c_queue_impl)

target_include_directories(word_grid_cpp PRIVATE CPP/inc)

