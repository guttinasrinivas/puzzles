cmake_minimum_required(VERSION 3.10)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

message(STATUS "Set TARGET_GPROF_ENABLED to enable gprof output")
if(DEFINED ENV{TARGET_GPROF_ENABLED})
    message(STATUS "Enabling gprof output")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pg")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg")
endif()

project(str2str)

if(NOT DEFINED ENV{VAS_LIB_HOME})
    message(FATAL_ERROR "Please export VAS_LIB_HOME to the correct path")
endif()


add_executable(word_ladder_c
               word_ladder/C/main.c
               $ENV{VAS_LIB_HOME}/src/ref_ctd.c)

add_executable(word_grid_c 
               word_grid/C/main.c
               word_grid/C/src/word_grid.c
               word_grid/C/src/word_list.c)

add_executable(word_ladder_cpp 
    word_ladder/CPP/src/main.cpp
    $ENV{VAS_LIB_HOME}/src/utils.cpp)


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

