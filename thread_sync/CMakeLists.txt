cmake_minimum_required(VERSION 3.12)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(LRUFileCacheSimulation VERSION 1.0.0 LANGUAGES CXX)

# Force modern C++ standard requirements
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT DEFINED ENV{VAS_LIB_HOME})
    message(FATAL_ERROR "Please export VAS_LIB_HOME to the correct path")
endif()

# Locate and configure the system multi-threading library (pthread, etc.)
find_package(Threads REQUIRED)

# Add the executable target (assumes your source code file is named main.cpp)
add_executable(threads_test src/thread_test.cpp)
target_compile_definitions(threads_test PRIVATE USE_VAS_LIB=1)
#target_compile_definitions(threads_test PRIVATE USE_CLI_ARGS=1)
target_include_directories(threads_test PRIVATE $ENV{VAS_LIB_HOME}/inc)
target_link_directories(threads_test PUBLIC $ENV{VAS_LIB_HOME}/build)

target_include_directories(threads_test PUBLIC src/inc src/tests/inc)

# Link the threads library to the target
target_link_libraries(threads_test PRIVATE simple_logger Threads::Threads)
if(NOT APPLE)
target_link_options(threads_test PRIVATE -static)
endif()

# Enable aggressive compiler warnings to keep the code tight and clean
if(MSVC)
    target_compile_options(threads_test PRIVATE /W4)
else()
    target_compile_options(threads_test PRIVATE -Wall -Wextra -Wpedantic)
endif()

