cmake_minimum_required(VERSION 3.10)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(str2str)


add_executable(word_ladder word_ladder/main.c)
add_executable(word_grid word_grid/main.c word_grid/src/word_grid.c word_grid/src/word_list.c)


if(NOT DEFINED ENV{VAS_LIB_HOME})
    message(FATAL_ERROR "Please export VAS_LIB_HOME to the correct path")
endif()

foreach (PROJ_NAME IN ITEMS word_ladder)
    message(STATUS "Setting options for ${PROJ_NAME}")
    target_include_directories(${PROJ_NAME} PRIVATE $ENV{VAS_LIB_HOME}/inc)
    target_link_directories(${PROJ_NAME} PUBLIC $ENV{VAS_LIB_HOME}/build)
    target_link_options(${PROJ_NAME} PRIVATE -static)
endforeach()

target_include_directories(word_ladder PRIVATE word_ladder)
target_link_libraries(word_ladder PRIVATE c_queue_impl wl_impl)

target_include_directories(word_grid PRIVATE word_grid/src/include)
