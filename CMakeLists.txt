cmake_minimum_required(VERSION 2.8)
project(Pore_Network_Generator)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -march=native -O2 -g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -march=native")
add_subdirectory(networkgen)



add_custom_command(TARGET Generator POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/config $<TARGET_FILE_DIR:MyTarget>)