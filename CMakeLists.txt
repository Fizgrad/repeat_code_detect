cmake_minimum_required(VERSION 3.20)
project(suffix_tree)

set(CMAKE_CXX_STANDARD 14)
include_directories(.)

add_executable(suffix_tree
        main.cpp
        SuffixTree.cpp
        SuffixTree.h
        SuffixTreeRepeatedInfos.cpp
        SuffixTreeRepeatedInfos.h InstructionHash.cpp InstructionHash.h ReadFile.cpp ReadFile.h
        FindSimpleRepeat.cpp
        FindSimpleRepeat.h Instruction.cpp Instruction.h)



