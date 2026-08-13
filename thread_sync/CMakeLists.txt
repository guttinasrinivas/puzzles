# Locate and configure the system multi-threading library (pthread, etc.)
find_package(Threads REQUIRED)

# Add the executable target (assumes your source code file is named main.cpp)
add_executable(threads_test src/thread_test.cpp)


# Choose CLI Args Parser
if(DEFINED USE_CLI_ARGS)
target_compile_definitions(threads_test PRIVATE USE_CLI_ARGS=1)
else()
target_compile_definitions(threads_test PRIVATE USE_VAS_LIB=1)
endif()


target_include_directories(threads_test PUBLIC src/inc src/tests/inc)
target_include_directories(threads_test PRIVATE $ENV{VAS_LIB_HOME}/inc)
target_link_directories(threads_test PUBLIC $ENV{VAS_LIB_HOME}/build)


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

