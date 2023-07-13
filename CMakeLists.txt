cmake_minimum_required(VERSION 3.5)
project(suffix_tree)

set(CMAKE_CXX_STANDARD 14)

find_package(Boost 1.58.0 REQUIRED COMPONENTS program_options)

add_executable(suffix_tree main.cpp SuffixTree.cpp SuffixTreeRepeatedInfos.cpp File.cpp FindSimpleRepeat.cpp Function.cpp Block.cpp Block.h)

target_link_libraries(suffix_tree PRIVATE Boost::program_options)
