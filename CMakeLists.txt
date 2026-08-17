cmake_minimum_required(VERSION 3.15)
project(MyMultiProjectWorkspace VERSION 1.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Force modern C++ standard requirements
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

message(STATUS "Set TARGET_GPROF_ENABLED to enable gprof output")
if(DEFINED ENV{TARGET_GPROF_ENABLED})
    message(STATUS "Enabling gprof output")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pg")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg")
endif()

project(vas_lc_puzzles)


if(NOT DEFINED ENV{VAS_LIB_HOME})
    message(FATAL_ERROR "Please export VAS_LIB_HOME to the correct path")
endif()


# Specify global output paths (Optional but highly recommended)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Add subdirectories containing child projects
add_subdirectory(word_ladder)
add_subdirectory(word_grid)
add_subdirectory(thread_sync)
add_subdirectory(remove_nth_node)

