cmake_minimum_required(VERSION 3.5)

project(suffix_tree)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 14)

# 设置默认构建类型为 Debug（如果未指定）
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# 根据操作系统设置 Capstone 和 Glog 的路径
if (APPLE)
    # MacOS 系统
    set(CAPSTONE_DIR /opt/homebrew/Cellar/capstone/5.0.3/)
    set(GLOG_DIR /opt/homebrew/Cellar/glog/0.6.0/)
else ()
    # 其他系统（假设为 Linux）
    set(GLOG_DIR /home/linuxbrew/.linuxbrew/Cellar/glog/0.6.0/)
    set(CAPSTONE_DIR /home/linuxbrew/.linuxbrew/Cellar/capstone/5.0.3)
endif ()

# 设置包含目录和库目录
set(CAPSTONE_INCLUDE_DIR ${CAPSTONE_DIR}/include)
set(CAPSTONE_LIB_DIR ${CAPSTONE_DIR}/lib)

set(GLOG_INCLUDE_DIR ${GLOG_DIR}/include)
set(GLOG_LIB_DIR ${GLOG_DIR}/lib)

# 包含 Capstone 和 Glog 的头文件
include_directories(${CAPSTONE_INCLUDE_DIR})
include_directories(${GLOG_INCLUDE_DIR})

# 链接 Capstone 和 Glog 的库目录
link_directories(${CAPSTONE_LIB_DIR})
link_directories(${GLOG_LIB_DIR})

# 在 Debug 模式下添加调试信息
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "启用调试信息")
    
    # 添加调试信息
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -fno-omit-frame-pointer")
endif()

# 添加可执行文件及其源文件
add_executable(suffix_tree 
    main.cpp 
    src/SuffixTree.cpp 
    src/SuffixTreeRepeatedInfos.cpp 
    src/File.cpp 
    src/FindSimpleRepeat.cpp 
    src/Function.cpp 
    src/Block.cpp 
    include/Block.h 
    include/Constants.h 
    include/Instruction.h 
    src/Instruction.cpp 
    src/ControlFlowGraph.cpp 
    include/ControlFlowGraph.h 
    src/Program.cpp 
    include/Program.h
)

# 链接 Capstone 和 Glog 库
target_link_libraries(suffix_tree PRIVATE capstone glog)