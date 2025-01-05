import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import glob
import os
import re

from formulas.second_term import calculate_second_coffi
from formulas.calculate_intercept import calculate_intercept

from graph_alpha_slope_all import prepare_slope_data
from graph_alpha_intercept_all import prepare_intercept_data






def extract_alpha(filename):
    match = re.search(r'(\d+(?:\.\d+)?)', os.path.basename(filename))
    if match:
        return float(match.group(1))
    return float('inf')

output_dir = "loglog_each"
os.makedirs(output_dir, exist_ok=True)

input_dir = "avg_F"
print(f"現在の読み込みデ\ィレクトリ: {input_dir}")

csv_files = sorted(glob.glob(f"{input_dir}/*.csv"), key=extract_alpha)

if not csv_files:
    print("CSVファイルが見つかりません。")
    exit()

global current_alpha, fig, ax
current_alpha = None
fig, ax = None, None

colors = ['blue', 'cyan', 'magenta', 'yellow', 'black', 'purple', 'orange']
markers = ['o', 's', '^', 'v', '<', '>', 'D']

slope_data = prepare_slope_data()

intercept_data= prepare_intercept_data()
from scipy import stats


def main():
    global current_alpha, fig, ax
    for i, csv_file in enumerate(csv_files):
        alpha = extract_alpha(csv_file)
        
            
        
        df = pd.read_csv(csv_file, header=None)
        l = df.iloc[:, 0].values
        F = df.iloc[:, 1].values
        
        print(l.tolist()[0])
        mask = (l > 0) & (F > 0)
        l = l[mask]
        F = F[mask]
        
        if len(l) == 0 or len(F) == 0:
            print(f"警告: {csv_file} には有効なデータがありません。スキップします。")
            continue
        

        
        log_l = np.log(l)
        log_F = np.log(F)
        
        # log_l > log(10^5.5)のデータのみを使用
        mask = log_l > np.log(10**5.5)
        log_l_filtered = log_l[mask]
        log_F_filtered = log_F[mask]
        
        coefficients = np.polyfit(log_l_filtered, log_F_filtered, 1)
        slope = coefficients[0]
        intercept = coefficients[1]
        print(f"{csv_file}: slope={slope:.4f}, intercept={intercept:.4f}")
        
        # データを保存
        if 'data' not in locals():
            data = {'alpha': [], 'slope': [], 'error': []}
        
        data['alpha'].append(alpha)
        data['slope'].append(slope)
        data['error'].append(0.1) # エラーは仮の値として0.1を設定
        
    # データフレームに変換
    data = pd.DataFrame(data)
    
    # 線形回帰を行う
    slope, intercept, r_value, p_value, std_err = stats.linregress(data['alpha'], data['slope'])

    # グラフを作成
    plt.figure(figsize=(12, 8))

    # 全データをプロット
    plt.plot(data['alpha'], data['slope'], 'o', color='blue', label='m', markersize=8)

    # フィッティング線を描画（横軸の範囲を拡張）
    x_fit = np.linspace(1.1, 2.0, 100)
    y_fit = slope * x_fit + intercept
    plt.plot(x_fit, y_fit, color='purple', label=f'y = {slope:.2f}x + {intercept:.2f}', linewidth=2)

    # 理論値の線を描画
    x_theory = np.linspace(1.0, 2.0, 100)  # alphaの範囲を1.0から2.0に設定
    y_theory = 1.5 - 0.5 * x_theory
    plt.plot(x_theory, y_theory, color='red', linestyle='--', label='y = 1.5 - 0.5x', linewidth=2)

    # alpha > 2 の範囲で y = 0.5 の直線を描画
    x_line = np.linspace(2, data['alpha'].max(), 100)
    y_line = np.full_like(x_line, 0.5)
    plt.plot(x_line, y_line, color='green', linestyle='--', label='y = 0.5', linewidth=2)

    plt.xlabel('Alpha', fontsize=30)
    plt.ylabel('Mean Slope', fontsize=30)
    plt.title('Alpha vs Mean Slope', fontsize=34)
    plt.legend(fontsize=26)
    plt.grid(True)

    # グラフの軸範囲を設定
    #plt.xlim(0.0, plt.xlim()[1])  
    plt.ylim(0.4, 1.1)  # y軸の最大値を1.1に設定

    # 軸のティックラベルのフォントサイズを大きくする
    plt.xticks(fontsize=16)
    plt.yticks(fontsize=16)

    # グラフを保存
    plt.savefig('graph_alpha_slope_all.png', dpi=300, bbox_inches='tight')
    #plt.show() #showoしないと保存されない。
    plt.close()

    print('グラフが graph_alpha_slope_all.png として保存されました。')

    print(slope)

if __name__ == "__main__":
    main()