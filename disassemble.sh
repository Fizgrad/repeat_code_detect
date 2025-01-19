#!/bin/bash

# 设置目录
input_dir="./so"
output_dir="./objdumps"

# 检查输出目录是否存在，不存在则创建
if [ ! -d "$output_dir" ]; then
    mkdir -p "$output_dir"
fi

# 遍历 .so 文件并执行 objdump
find "$input_dir" -type f -name "*.so" | while read so_file; do
    # 获取文件名（不带路径）
    file_name=$(basename "$so_file")
    
    # 构建输出文件的路径
    output_file="$output_dir/${file_name%.so}.txt"
    
    # 执行 objdump 反汇编并输出到目标文件
    aarch64-linux-gnu-objdump --architecture=arm64 -d "$so_file" > "$output_file"
    
    # 打印操作信息
    echo "Processed: $so_file -> $output_file"
done

echo "All files have been processed."