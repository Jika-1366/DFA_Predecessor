import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import glob
import os
import re

from formulas.second_term import calculate_second_coffi
from graph_alpha_intercept_all_between1and2 import theoretical_value
from graph_alpha_slope_all import prepare_slope_data
from graph_alpha_intercept_all_above2 import prepare_data_above2

def calculate_F_calc_simple(l, alpha):
    global main_coffi
    sqrt_log_main_coffi = theoretical_value(alpha)
    sqrt_main_coffi = np.exp(sqrt_log_main_coffi)
    main_coffi = sqrt_main_coffi**2.0
    F2 = (main_coffi * l**(3-alpha))
    F = np.sqrt(F2)
    print("main_coffi: "+str(np.sqrt(main_coffi)))
    return F

def calculate_F_calc_detailed(l, alpha):
    global main_coffi, second_coffi
    sqrt_log_main_coffi = theoretical_value(alpha)
    sqrt_main_coffi =  np.exp(sqrt_log_main_coffi)
    main_coffi = sqrt_main_coffi**2.0
    second_coffi = calculate_second_coffi(alpha=alpha)
    F2= (main_coffi*l**((3-alpha)) + second_coffi*l**(4-2*alpha))
    F = np.sqrt(F2)
    return F


def calcualte_F_calc_alpha_above2(l, alpha):
    global coffi_alpha_above2
    mu = alpha / (alpha - 1)
    
    coffi_alpha_above2 =  (
            1 / (15 * mu)
    )

    F2 =  (
        coffi_alpha_above2*l
    )
    F = np.sqrt(F2)
    return F


def calculate_F_calced_dfa3(l, alpha):
    global slope, intercept
    slope_data_row = slope_data[slope_data['alpha'] == alpha]
    slope = slope_data_row['mean_slope'].iloc[0]
    intercept_data_row = intercept_data_above2[intercept_data_above2['alpha'] == alpha]
    intercept = intercept_data_row['mean_intercept'].iloc[0]
    slope = slope
    intercept = np.exp(intercept)
    F =  intercept*(l**slope)
    return F


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

slope_data = prepare_slope_data()
intercept_data_above2= prepare_data_above2()



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
    
    print(f"{csv_file}: slope={slope:.4f}, intercept={intercept:.4f}")
    
    fit_line = np.exp(intercept) * l ** slope
    ax.loglog(l, fit_line, linestyle='--', color='blue', 
              label=f'Fit: F = {np.exp(intercept):.4f} * l^{slope:.4f}')
    

    global main_coffi, second_coffi, coffi_alpha_above2
    # 計算値 F_calc_simple と F_calc_complex を追加（1 < alpha < 2 の場合のみ）
    if 1 < alpha < 2:
        F_calc_simple = calculate_F_calc_simple(l, alpha)
        ax.loglog(l, F_calc_simple, linestyle='-', color='red', linewidth=1.5,
                  label=f'Calc Simple: F = {np.sqrt(main_coffi):.3f}*l^({(3-alpha)/2:.3f})')
        F_calc_complex = calculate_F_calc_detailed(l, alpha)
        ax.loglog(l, F_calc_complex, linestyle='-', color='green', linewidth=1.5,
                  label=f'Calc Complex: F = {np.sqrt(main_coffi):.3f}l^({(3-alpha)/2}) + {np.sqrt(second_coffi):.3f}*l^({2-alpha:.3f})')
    # alpha > 2 の場合の処理を追加
    elif alpha > 2:
        F_calc_above2 = calcualte_F_calc_alpha_above2(l, alpha)
        F_dfa3 = calculate_F_calced_dfa3(l, alpha)
        ax.loglog(l, F_calc_above2, linestyle='-', color='purple', linewidth=1.5,
                  label=f'Calc (α > 2): F = {coffi_alpha_above2:.3f}*l^0.5')
        ax.loglog(l, F_dfa3, linestyle='-', color='orange', linewidth=1.5,
                  label=f'Calc DFA3: F = {000:.3f}*l^0.5')
        # y軸の範囲を更新
        ax.set_ylim([min(min(F), min(F_calc_above2)), 
                     max(max(F), max(F_calc_above2))])
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
    if alpha == 1.2:
        # l = 100のときのそれぞれの値を計算してプリント
        l_100 = 10**6

        F_100 = np.exp(intercept) * l_100 ** slope
        F_calc_simple_100 = calculate_F_calc_simple(l_100, alpha)
        F_calc_complex_100 = calculate_F_calc_detailed(l_100, alpha)
        
        print(f"α = {alpha:.1f}, l = 100 のときの値:")
        print(f"  フィッティング線: F = {F_100:.4f}")
        print(f"  単純計算値: F = {F_calc_simple_100:.4f}")
        print(f"  詳細計算値: F = {F_calc_complex_100:.4f}")

        

if fig is not None:
    plt.savefig(f"{output_dir}/graph_alpha_{current_alpha:.1f}.png")
    plt.close(fig)

print("全てのグラフが生成されました。")