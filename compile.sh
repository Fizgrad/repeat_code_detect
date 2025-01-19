#!/bin/bash

# 设置项目根目录
PROJECT_ROOT=$(pwd)

# 检查并创建 build 目录
if [ ! -d "$PROJECT_ROOT/build" ]; then
    echo "Creating build directory..."
    mkdir "$PROJECT_ROOT/build"
else
    echo "Build directory already exists."
fi

# 进入 build 目录
cd "$PROJECT_ROOT/build" || exit

# 运行 cmake 配置项目
echo "Running cmake with Debug configuration..."
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DCMAKE_BUILD_TYPE=Debug ..

# 获取 CPU 核心数
CPU_CORES=$(nproc)

# 使用 make 并行构建
echo "Building with $CPU_CORES CPU cores..."
make -j$CPU_CORES

# 提示完成
echo "Build completed successfully!"