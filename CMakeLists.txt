cmake_minimum_required(VERSION 3.5)

project(suffix_tree)

set(CMAKE_CXX_STANDARD 14)

if (APPLE)
    # MacOS 系统
    set(CAPSTONE_DIR /opt/homebrew/Cellar/capstone/4.0.2/)
else ()
    set(CAPSTONE_DIR /home/linuxbrew/.linuxbrew/Cellar/capstone/4.0.2)
endif ()

set(CAPSTONE_INCLUDE_DIR ${CAPSTONE_DIR}/include)
set(CAPSTONE_LIB_DIR ${CAPSTONE_DIR}/lib)

include_directories(${CAPSTONE_INCLUDE_DIR})

link_directories(${CAPSTONE_LIB_DIR})

find_package(Boost 1.58.0 REQUIRED COMPONENTS program_options)

add_executable(suffix_tree main.cpp src/SuffixTree.cpp src/SuffixTreeRepeatedInfos.cpp src/File.cpp src/FindSimpleRepeat.cpp src/Function.cpp src/Block.cpp include/Block.h include/Constant.h include/Instruction.h src/Instruction.cpp src/ControlFlowGraph.cpp include/ControlFlowGraph.h src/Program.cpp include/Program.h)

target_link_libraries(suffix_tree PRIVATE capstone)

target_link_libraries(suffix_tree PRIVATE Boost::program_options)
