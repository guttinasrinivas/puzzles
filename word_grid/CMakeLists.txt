add_executable(word_grid_c 
               C/main.c
               C/src/word_grid.c
               $ENV{VAS_LIB_HOME}/src/ref_ctd.c)

foreach (PROJ_NAME IN ITEMS word_grid_c)
    message(STATUS "Setting options for ${PROJ_NAME}")
    target_include_directories(${PROJ_NAME} PRIVATE $ENV{VAS_LIB_HOME}/inc)
    target_link_directories(${PROJ_NAME} PUBLIC $ENV{VAS_LIB_HOME}/build)
    if(NOT APPLE)
        target_link_options(${PROJ_NAME} PRIVATE -static)
    endif()
endforeach()

target_include_directories(word_grid_c PRIVATE C/src/include)
target_link_libraries(word_grid_c PRIVATE c_queue_impl wl_impl)

