import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import glob
import os
import re

from formulas.second_term import calculate_second_coffi
from graph_alpha_intercept_all_between1and2 import theoretical_value
from loglog_each import calcualte_F_calc_alpha_above2, calculate_F_calc_detailed, calculate_F_calc_simple, input_dir

def extract_alpha(filename):
    match = re.search(r'(\d+(?:\.\d+)?)', os.path.basename(filename))
    if match:
        return float(match.group(1))
    return float('inf')

output_dir = "linear_each"
os.makedirs(output_dir, exist_ok=True)


print(f"現在の読み込みディレクトリ: {input_dir}")
print("正常ですね")

csv_files = sorted(glob.glob(f"{input_dir}/*.csv"), key=extract_alpha)

if not csv_files:
    print("CSVファイルが見つかりません。")
    exit()

current_alpha = None
fig, ax = None, None

colors = ['blue', 'cyan', 'magenta', 'yellow', 'black', 'purple', 'orange']
markers = ['o', 's', '^', 'v', '<', '>', 'D']


for i, csv_file in enumerate(csv_files):
    alpha = extract_alpha(csv_file)
    
    if alpha != current_alpha:
        if fig is not None:
            save_path = f"{output_dir}/graph_alpha_{current_alpha:.1f}.png"
            plt.savefig(save_path)
            plt.close(fig)
        
        fig, ax = plt.subplots(figsize=(10, 6))
        current_alpha = alpha
        color_index = 0
    
    df = pd.read_csv(csv_file)
    l = df.iloc[:, 0].values
    F = df.iloc[:, 1].values
    
    mask = (l > 0) & (F > 0)
    l = l[mask]
    F = F[mask]
    
    if len(l) == 0 or len(F) == 0:
        print(f"警告: {csv_file} には有効なデータがありません。スキップします。")
        continue
    
    ax.plot(l, F, marker=markers[color_index % len(markers)], linestyle='', 
            label=f'Data_{os.path.basename(csv_file)}', color=colors[color_index % len(colors)])
    

    # 計算値 F_calc_simple と F_calc_complex を追加（1 < alpha < 2 の場合のみ）
    if 1 < alpha < 2:
        F_calc_simple = calculate_F_calc_simple(l, alpha)
        ax.plot(l, F_calc_simple, linestyle='-', color='red', linewidth=1.5,
                label=f'Calc Simple')
        F_calc_complex = calculate_F_calc_detailed(l, alpha)
        ax.plot(l, F_calc_complex, linestyle='-', color='green', linewidth=1.5,
                label=f'Calc Complex')
        
    # alpha > 2 の場合の処理を追加
    elif alpha > 2:
        F_calc_above2 = calcualte_F_calc_alpha_above2(l, alpha)
        ax.plot(l, F_calc_above2, linestyle='-', color='orange', linewidth=1.5,
                label=f'Calc (α > 2)')
        
    color_index += 1

    ax.set_title(f"Linear scale graph (α = {alpha:.1f})", fontsize=18)
    ax.set_xlabel("l", fontsize=16)
    ax.set_ylabel("F", fontsize=16)
    ax.legend(fontsize=10)
    ax.set_xlim([min(l), max(l)])
    ax.grid(True)
    
    # 線形スケールを明示的に設定
    ax.set_xscale('linear')
    ax.set_yscale('linear')
    
    # y軸の範囲を設定
    if 1 < alpha < 2:
        ax.set_ylim([min(min(F), min(F_calc_simple), min(F_calc_complex)), 
                     max(max(F), max(F_calc_simple), max(F_calc_complex))])
    elif alpha > 2:
        ax.set_ylim([min(min(F), min(F_calc_above2)), 
                     max(max(F), max(F_calc_above2))])
    else:
        ax.set_ylim([min(F), max(F)])


if fig is not None:
    save_path = f"{output_dir}/graph_alpha_{current_alpha:.1f}.png"
    plt.savefig(save_path)
    print(f"グラフを保存しました: {save_path}")  # 保存場所を報告
    plt.close(fig)

print("全てのグラフが生成されました。")