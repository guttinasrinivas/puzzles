add_executable(word_ladder_c
               C/main.c
               $ENV{VAS_LIB_HOME}/src/ref_ctd.c)

add_executable(word_ladder_cpp 
    CPP/src/main.cpp
    $ENV{VAS_LIB_HOME}/src/utils.cpp)


foreach (PROJ_NAME IN ITEMS word_ladder_c word_ladder_cpp)
    message(STATUS "Setting options for ${PROJ_NAME}")
    target_include_directories(${PROJ_NAME} PRIVATE $ENV{VAS_LIB_HOME}/inc)
    target_link_directories(${PROJ_NAME} PUBLIC $ENV{VAS_LIB_HOME}/build)
    if(NOT APPLE)
        target_link_options(${PROJ_NAME} PRIVATE -static)
    endif()
endforeach()

target_include_directories(word_ladder_c PRIVATE C)
target_link_libraries(word_ladder_c PRIVATE c_queue_impl wl_impl)

target_include_directories(word_ladder_cpp PRIVATE CPP/inc word_ladder_cpp/CPP)
target_link_libraries(word_ladder_cpp PRIVATE wl_impl simple_logger)

