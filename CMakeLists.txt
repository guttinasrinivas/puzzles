cmake_minimum_required(VERSION 3.10)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(str2str)


add_executable(word_ladder_c word_ladder/C/main.c)
add_executable(word_grid_c word_grid/C/main.c word_grid/C/src/word_grid.c word_grid/C/src/word_list.c)

add_executable(word_ladder_cpp 
    word_ladder/CPP/src/main.cpp)


if(NOT DEFINED ENV{VAS_LIB_HOME})
    message(FATAL_ERROR "Please export VAS_LIB_HOME to the correct path")
endif()

foreach (PROJ_NAME IN ITEMS word_ladder_c word_ladder_cpp)
    message(STATUS "Setting options for ${PROJ_NAME}")
    target_include_directories(${PROJ_NAME} PRIVATE $ENV{VAS_LIB_HOME}/inc)
    target_link_directories(${PROJ_NAME} PUBLIC $ENV{VAS_LIB_HOME}/build)
    target_link_options(${PROJ_NAME} PRIVATE -static)
endforeach()

target_include_directories(word_ladder_c PRIVATE word_ladder/C)
target_link_libraries(word_ladder_c PRIVATE c_queue_impl wl_impl)

target_include_directories(word_ladder_cpp PRIVATE word_ladder/CPP/inc word_ladder_cpp/CPP)
target_link_libraries(word_ladder_cpp PRIVATE wl_impl simple_logger)

target_include_directories(word_grid_c PRIVATE word_grid/C/src/include)

