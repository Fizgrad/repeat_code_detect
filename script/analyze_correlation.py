#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os
import glob
import pandas as pd
from tabulate import tabulate
import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np
from scipy import stats

def parse_file(file_path):
    """
    解析单个文件，提取所需的数据。

    参数:
        file_path (str): 文件的完整路径。

    返回:
        dict: 包含文件名和所需计数的数据字典。
    """
    data = {
        'SO File': os.path.basename(file_path),
        'Code Count': 0,
        'Predict Benefits': 0
    }
    try:
        with open(file_path, 'r') as f:
            for line in f:
                line = line.strip()
                if line.startswith('Code Count'):
                    data['Code Count'] = int(line.split(':')[1].strip())
                elif line.startswith('Predict Benefits'):
                    data['Predict Benefits'] = int(line.split(':')[1].strip())
    except Exception as e:
        print(f"Error parsing file {file_path}: {e}")
    return data

def analyze_correlation(df, output_path="code_count_vs_predict_benefits.png"):
    """
    分析 log(Code Count) 与 Predict Benefits / Code Count 之间的相关性，并生成散点图。

    参数:
        df (DataFrame): 包含所有数据的 DataFrame。
        output_path (str): 图像保存路径。
    """
    # 计算 log(Code Count)
    df['log_Code Count'] = df['Code Count'].apply(lambda x: np.log(x) if x > 0 else 0)

    # 计算 Predict Benefits / Code Count
    df['Predict Benefits / Code Count'] = df.apply(
        lambda row: row['Predict Benefits'] / row['Code Count'] if row['Code Count'] != 0 else 0,
        axis=1
    )

    # 计算相关系数和 p 值
    correlation, p_value = stats.pearsonr(df['log_Code Count'], df['Predict Benefits / Code Count'])
    print(f"\n相关系数 (Pearson): {correlation:.4f}")
    print(f"P 值: {p_value:.4e}")

    # 设置 Seaborn 风格
    sns.set(style="whitegrid")

    # 创建散点图并添加回归线
    plt.figure(figsize=(10, 6))
    scatter_plot = sns.regplot(
        x='log_Code Count', 
        y='Predict Benefits / Code Count', 
        data=df, 
        scatter_kws={'s': 50, 'alpha':0.7}, 
        line_kws={'color': 'red'}
    )

    # 添加标题和标签
    plt.title('Correlation between log(Code Count) and Predict Benefits / Code Count', fontsize=16)
    plt.xlabel('log(Code Count)', fontsize=14)
    plt.ylabel('Predict Benefits / Code Count', fontsize=14)

    # 显示相关系数和 p 值在图表上
    plt.text(
        0.05, 0.95, 
        f'Pearson r = {correlation:.4f}\nP-value = {p_value:.4e}', 
        transform=plt.gca().transAxes, 
        fontsize=12,
        verticalalignment='top', 
        bbox=dict(boxstyle='round', facecolor='white', alpha=0.5)
    )

    # 保存图形
    plt.tight_layout()
    plt.savefig(output_path, dpi=300, bbox_inches='tight')
    print(f"相关性分析图已保存到 {output_path}")

    # 显示图形
    plt.show()

def main():
    import numpy as np  # 导入放在函数内部，避免在全局未定义

    # 设置目录路径
    directory = "/home/david/repeat_code_detect/out"
    if not os.path.isdir(directory):
        print(f"目录不存在: {directory}")
        return

    # 查找所有匹配的文件（仅单个 SO 文件，无需前缀处理）
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

    # 检查是否有有效的数据
    if df.empty:
        print("没有有效的数据可供分析。")
        return

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

    # 进行相关性分析
    analyze_correlation(df, output_path="code_count_vs_predict_benefits.pdf")

if __name__ == "__main__":
    main()