import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import glob
import os
import re

from formulas.second_term import calculate_second_coffi
from graph_alpha_intercept_all_between1and2 import theoretical_value


def calculate_F_calc_simple(l, alpha):
    log_coffi = theoretical_value(alpha)
    coffi =  np.exp(log_coffi)
    second_coffi = calculate_second_coffi(alpha=alpha)
    return coffi*l**((3-alpha)/2) 

def calculate_F_calc_detailed(l, alpha):
    log_coffi = theoretical_value(alpha)
    coffi =  np.exp(log_coffi)
    second_coffi = calculate_second_coffi(alpha=alpha)
    return coffi*l**((3-alpha)/2) + second_coffi*l**(2-alpha)



def extract_alpha(filename):
    match = re.search(r'(\d+(?:\.\d+)?)', os.path.basename(filename))
    if match:
        return float(match.group(1))
    return float('inf')

output_dir = "loglog_each"
os.makedirs(output_dir, exist_ok=True)

input_dir = "avg_F"
print(f"現在の読み込みディレクトリ: {input_dir}")

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
            plt.savefig(f"{output_dir}/graph_alpha_{current_alpha:.1f}.png")
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
    
    ax.loglog(l, F, marker=markers[color_index % len(markers)], linestyle='', 
              label=f'Data_{os.path.basename(csv_file)}', color=colors[color_index % len(colors)])
    
    log_l = np.log(l)
    log_F = np.log(F)
    coefficients = np.polyfit(log_l, log_F, 1)
    slope = coefficients[0]
    intercept = coefficients[1]
    
    print(f"{csv_file}: slope={slope:.2f}, intercept={intercept:.2f}")
    
    fit_line = np.exp(intercept) * l ** slope
    ax.loglog(l, fit_line, linestyle='--', color='blue', 
              label=f'Fit: F = {np.exp(intercept):.2e} * l^{slope:.2f}')
    

    # 計算値 F_calc_simple と F_calc_complex を追加（1 < alpha < 2 の場合のみ）
    if 1 < alpha < 2:
        F_calc_simple = calculate_F_calc_simple(l, alpha)
        ax.loglog(l, F_calc_simple, linestyle='-', color='red', linewidth=1.5,
                  label=f'Calc Simple: F = C1*l^({(3-alpha)/2:.2f})')
        
        F_calc_complex = calculate_F_calc_detailed(l, alpha)
        ax.loglog(l, F_calc_complex, linestyle='-', color='green', linewidth=1.5,
                  label=f'Calc Complex: F = C2l^({(3-alpha)/2:.2f}) + C3*l^({2-alpha:.2f})')
    

    color_index += 1

    ax.set_title(f"Log-log graph (α = {alpha:.1f})", fontsize=18)
    ax.set_xlabel("l", fontsize=16)
    ax.set_ylabel("F", fontsize=16)
    ax.legend(fontsize=10)
    ax.set_xlim([min(l), max(l)])
    ax.grid(True)
    
    # y軸の範囲を設定（F_calc_simple と F_calc_complex が存在する場合のみ考慮）
    if 1 < alpha < 2:
        ax.set_ylim([min(min(F), min(F_calc_simple), min(F_calc_complex)), 
                     max(max(F), max(F_calc_simple), max(F_calc_complex))])
    else:
        ax.set_ylim([min(F), max(F)])

if fig is not None:
    plt.savefig(f"{output_dir}/graph_alpha_{current_alpha:.1f}.png")
    plt.close(fig)

print("全てのグラフが生成されました。")