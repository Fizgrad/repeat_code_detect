#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os
import glob
import pandas as pd
from tabulate import tabulate
import matplotlib.pyplot as plt
from matplotlib.patches import Patch

prefix_list = [
    "libeditorkit",
    "libohos.",
    "libcommonmark4cj",
    "libohos_app_cangjie_entry",
    "libprism4cj"
]

def parse_file(file_path):
    """
    解析单个文件，提取所需的数据。

    参数:
        file_path (str): 文件的完整路径。

    返回:
        dict: 包含文件名和四个计数的数据字典。
    """
    data = {
        'SO File': os.path.basename(file_path)[0:-4],
        'Basic Block Count': 0,
        'Function Count': 0,
        'Code Count': 0,
        'Predict Benefits': 0
    }
    try:
        with open(file_path, 'r') as f:
            for line in f:
                line = line.strip()
                if line.startswith('Basic Block Count'):
                    data['Basic Block Count'] = int(line.split(':')[1].strip())
                elif line.startswith('Function Count'):
                    data['Function Count'] = int(line.split(':')[1].strip())
                elif line.startswith('Code Count'):
                    data['Code Count'] = int(line.split(':')[1].strip())
                elif line.startswith('Predict Benefits'):
                    data['Predict Benefits'] = int(line.split(':')[1].strip())
    except Exception as e:
        print(f"Error parsing file {file_path}: {e}")
    return data

def plot_optimization_ratio(plot_df, output_path="optimization_ratio.pdf"):
    """
    绘制预计优化比例的图形。

    参数:
        plot_df (DataFrame): 包含所有绘图数据。
        output_path (str): 图像保存路径。
    """
    # 按照 'Predict Benefits / Code Count' 从大到小排序
    plot_df_sorted = plot_df.sort_values(by='Predict Benefits / Code Count', ascending=False)
    print(plot_df)
    print(plot_df_sorted)
 # 定义颜色映射
    color_map = {
        'Individual': 'skyblue',
        'Prefix Summaries': 'orange',
        'Total': 'green'
    }

    # 分配类别
    plot_df_sorted['Category'] = plot_df_sorted['SO File'].apply(
        lambda x: 'Total' if x == 'Total' else ('Prefix Summaries' if x.endswith('*') else 'Individual')
    )

    # 根据类别分配颜色
    plot_df_sorted['Color'] = plot_df_sorted['Category'].map(color_map)

    plt.figure(figsize=(40,8))  # 动态调整高度

    # 绘制条形图
    bars = plt.bar(plot_df_sorted['SO File'], plot_df_sorted['Predict Benefits / Code Count'], color=plot_df_sorted['Color'])

    # 添加数据标签
    for bar in bars:
        height = bar.get_height()
        plt.annotate(f'{height:.2f}',
                     xy=(bar.get_x() + bar.get_width() / 2, height),
                     xytext=(0, 3),  # 3 points vertical offset
                     textcoords="offset points",
                     ha='center', va='bottom', fontsize=4)

    # 添加标题和标签
    plt.title('Predict Benefits / Code Count', fontsize=16)
    plt.xlabel('SO File / Summary', fontsize=14)
    plt.ylabel('Optimization Ratio', fontsize=14)

    # 设置 x 轴标签旋转角度
    plt.xticks(rotation=90, ha='right', fontsize=10)

    # 创建自定义图例
    legend_elements = [
        Patch(facecolor=color_map['Individual'], label='Individual SO Files'),
        Patch(facecolor=color_map['Prefix Summaries'], label='Prefix Summaries'),
        Patch(facecolor=color_map['Total'], label='Total')
    ]
    plt.legend(handles=legend_elements, fontsize=12)


    # 调整布局
    plt.tight_layout()

    # 保存图形
    plt.savefig(output_path, dpi=300)
    print(f"\n预计优化比例图已保存到 {output_path}")

    # 显示图形
    plt.show()

def main():
    # 设置目录路径
    directory = "/home/david/repeat_code_detect/out"
    if not os.path.isdir(directory):
        print(f"目录不存在: {directory}")
        return

    # 查找所有匹配的文件
    pattern = "lib*.txt"
    search_pattern = os.path.join(directory, pattern)
    files = glob.glob(search_pattern)

    if not files:
        print(f"No files found matching pattern {pattern} in directory {directory}")
        return

    # 解析所有文件
    records = []
    for file_path in files:
        record = parse_file(file_path)
        records.append(record)

    # 创建 DataFrame
    df = pd.DataFrame(records)

    # 计算 Predict Benefits / Code Count 比值
    df['Predict Benefits / Code Count'] = df.apply(
        lambda row: row['Predict Benefits'] / row['Code Count'] if row['Code Count'] != 0 else 0,
        axis=1
    )

    # 输出每个 SO 文件的数据
    print("\n每个 SO 文件的数据:")
    print(tabulate(df, headers='keys', tablefmt='grid', showindex=False))

    # 保存到 CSV 文件
    output_csv = "so_files_summary.csv"
    df.to_csv(output_csv, index=False)
    print(f"\n详细数据已保存到 {output_csv}")

    # 处理特定前缀的文件
    prefix_summaries = []
    for prefix in prefix_list:
        specific_pattern = f"{prefix}*.txt"
        specific_files = [f for f in files if os.path.basename(f).startswith(prefix)]

        if specific_files:
            specific_records = [record for record in records if record['SO File'].startswith(prefix)]
            df_specific = pd.DataFrame(specific_records)

            # 计算总和
            sum_basic_block = df_specific['Basic Block Count'].sum()
            sum_function = df_specific['Function Count'].sum()
            sum_code = df_specific['Code Count'].sum()
            sum_predict_benefits = df_specific['Predict Benefits'].sum()
            ratio_specific = sum_predict_benefits / sum_code if sum_code != 0 else 0

            specific_summary = {
                'SO File': f"{prefix}*",
                'Basic Block Count': sum_basic_block,
                'Function Count': sum_function,
                'Code Count': sum_code,
                'Predict Benefits': sum_predict_benefits,
                'Predict Benefits / Code Count': ratio_specific
            }

            prefix_summaries.append(specific_summary)

            print(f"\n{prefix}前缀文件的数据总和:")
            print(tabulate([specific_summary], headers='keys', tablefmt='grid'))

            # 保存特定前缀的数据到 CSV
            output_specific_csv = f"{prefix}_summary.csv"
            df_specific.to_csv(output_specific_csv, index=False)
            print(f"\n{prefix}前缀的数据已保存到 {output_specific_csv}")
        else:
            print(f"\n未找到以 {prefix} 开头的文件。")

    # 计算所有文件的总和
    total_basic_block = df['Basic Block Count'].sum()
    total_function = df['Function Count'].sum()
    total_code = df['Code Count'].sum()
    total_predict_benefits = df['Predict Benefits'].sum()
    ratio_total = total_predict_benefits / total_code if total_code != 0 else 0

    total_summary = {
        'SO File': 'Total',
        'Basic Block Count': total_basic_block,
        'Function Count': total_function,
        'Code Count': total_code,
        'Predict Benefits': total_predict_benefits,
        'Predict Benefits / Code Count': ratio_total
    }

    print("\n所有文件的数据总和:")
    print(tabulate([total_summary], headers='keys', tablefmt='grid'))

    # 保存总和到 CSV
    output_total_csv = "total_summary.csv"
    df_total = pd.DataFrame([total_summary])
    df_total.to_csv(output_total_csv, index=False)
    print(f"\n所有文件的总和已保存到 {output_total_csv}")

    # 准备绘图数据
    # 包含每个 SO 文件
    individual_ratios = df[['SO File', 'Predict Benefits / Code Count']].copy()

    # 包含每个前缀的总和
    prefix_ratios = pd.DataFrame(prefix_summaries)

    # 包含总和
    total_ratio = pd.DataFrame([total_summary])

    # 合并所有数据用于绘图
    plot_df = pd.concat([individual_ratios, prefix_ratios, total_ratio], ignore_index=True)

    # 绘制图形
    plot_optimization_ratio(plot_df)

if __name__ == "__main__":
    main()