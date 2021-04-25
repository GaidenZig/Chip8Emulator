cmake_minimum_required(VERSION 3.0)
project(CHIP-8)

file (GLOB SOURCES "src/*.cpp")
set(BIN_DIR ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})


find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})
add_executable(CHIP-8 ${SOURCES})
target_link_libraries(CHIP-8 ${SDL2_LIBRARIES})

install(TARGETS CHIP-8 RUNTIME DESTINATION ${BIN_DIR})

# function(echo_all_cmake_variable_values)
#     message(STATUS “”)
#     get_cmake_property(vs VARIABLES)
#     foreach(v ${vs})
#         message(STATUS “${v}=’${${v}}'”)
#     endforeach(v)
#     message(STATUS “”)
# endfunction()

# echo_all_cmake_variable_values()