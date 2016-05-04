cmake_minimum_required(VERSION 2.8.11)

project(str2str)



include_directories(
    /home/gsjy/src/cpp/codelite/practice/str2str/
    /home/gsjy/src/cpp/codelite/practice/str2str/

)

# Linker options
set(CMAKE_LDFLAGS "${CMAKE_LDFLAGS}  ")

# Library path
set(CMAKE_LDFLAGS "${CMAKE_LDFLAGS} -L\".\" ")

# Define the C sources
set ( C_SRCS
    main.c
    queue_impl.c
)

set_source_files_properties(
    ${C_SRCS} PROPERTIES COMPILE_FLAGS 
    " -g -O0 -Wall")

add_executable(str2str ${CXX_SRCS} ${C_SRCS})

