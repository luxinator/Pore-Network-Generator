cmake_minimum_required(VERSION 2.8)
project(Pore_Network_Generator)

set(SOURCE_FILES
    ArrayFunctions.cpp  Combinator.cpp  Eliminator.cpp  inputParser.cpp  main.cpp  PoreNetwork.cpp  vtk.cpp  writerFunctions.cpp
    ArrayFunctions.h  Combinator.h  Eliminator.h  inputParser.h  PoreNetwork.h  vtk.h  writerFunctions.h)
add_executable(Generator ${SOURCE_FILES})
