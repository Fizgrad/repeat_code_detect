#!/bin/bash

# 获取所有反汇编的 .txt 文件
input_dir="./objdumps"
output_dir="./out"

# 确保输出目录存在
mkdir -p "$output_dir"

# 遍历 ./objdumps 目录下的所有 .txt 文件
for txt_file in "$input_dir"/*.txt
do
    # 获取文件名（不带路径）
    filename=$(basename "$txt_file")
    
    # 构建输出文件路径
    output_file="$output_dir/$filename"
    
    # 调用 suffix_tree 进行处理
    ./build/suffix_tree --mode origin --code "$txt_file" --out "$output_file" > ./log/"$filename".log
    
    # 打印当前处理的文件信息
    echo "Processed: $txt_file -> $output_file"
done

echo "All files have been processed."