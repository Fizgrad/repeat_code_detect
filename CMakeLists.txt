cmake_minimum_required(VERSION 3.5)

project(suffix_tree)

set(CMAKE_CXX_STANDARD 14)

if (APPLE)
    # MacOS 系统
    set(CAPSTONE_DIR /opt/homebrew/Cellar/capstone/4.0.2/)
    set(GLOG_DIR /opt/homebrew/Cellar/glog/0.6.0/)
else ()
    set(GLOG_DIR /home/linuxbrew/.linuxbrew/Cellar/glog/0.6.0/)
    set(CAPSTONE_DIR /home/linuxbrew/.linuxbrew/Cellar/capstone/4.0.2)
endif ()

set(CAPSTONE_INCLUDE_DIR ${CAPSTONE_DIR}/include)
set(CAPSTONE_LIB_DIR ${CAPSTONE_DIR}/lib)

set(GLOG_INCLUDE_DIR ${GLOG_DIR}/include)
set(GLOG_LIB_DIR ${GLOG_DIR}/lib)


include_directories(${CAPSTONE_INCLUDE_DIR})

link_directories(${CAPSTONE_LIB_DIR})


include_directories(${GLOG_INCLUDE_DIR})

link_directories(${GLOG_LIB_DIR})

find_package(Boost 1.58.0 REQUIRED COMPONENTS program_options)

add_executable(suffix_tree main.cpp src/SuffixTree.cpp src/SuffixTreeRepeatedInfos.cpp src/File.cpp src/FindSimpleRepeat.cpp src/Function.cpp src/Block.cpp include/Block.h include/Constant.h include/Instruction.h src/Instruction.cpp src/ControlFlowGraph.cpp include/ControlFlowGraph.h src/Program.cpp include/Program.h)

target_link_libraries(suffix_tree PRIVATE capstone)

target_link_libraries(suffix_tree PRIVATE glog)

target_link_libraries(suffix_tree PRIVATE Boost::program_options)
