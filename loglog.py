"""
# このファイルは、前に作成したlとFの関係を表すcsvファイルを読み込んで、
# 両対数グラフにプロットし、線形フィッティングを行うプログラムです。
"""

import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import glob


# beki_walk_alpha_*.csvファイルを取得
csv_files = glob.glob("l_F_beki_walk_alpha_*.csv")

for csv_file in csv_files:
    # CSVファイルを読み込む
    df = pd.read_csv(csv_file)

    # lとFの値を取得
    l = df['l'].values
    F = df['F'].values

    # 両対数グラフを作成
    plt.figure(figsize=(10, 6))
    #plt.loglog(l, F, marker='o', label='Data') 
    plt.plot(np.log(l), np.log(F), marker='o', label='Data')

    # 直線フィッティング
    log_l = np.log(l)
    log_F = np.log(F)
    coefficients = np.polyfit(log_l, log_F, 1)
    slope = coefficients[0]
    intercept = coefficients[1]
    print(f"{csv_file}: slope={slope:.2f}, intercept={intercept:.2f}")

    fit_line = np.exp(intercept) * l ** slope
    #plt.loglog(l, fit_line, linestyle='--', color='red', label='Fit Line')
    plt.plot(np.log(l), np.log(fit_line), linestyle='--', color='red', label='Fit Line')

    # フィッティングの式をグラフに表示
    plt.text(0.1, 0.8, f'F = {np.exp(intercept):.2f} * l^{slope:.2f}', transform=plt.gca().transAxes)

    # グラフのタイトル、軸ラベル、凡例を設定
    plt.title(f"Log-log graph and linear fitting ({csv_file})")
    plt.xlabel("l")
    plt.ylabel("F")
    plt.legend()

    # グリッドを表示
    plt.grid(True)

    # グラフを保存
    plt.savefig(f"Log-log graph and linear fitting ({csv_file}).png")

    # グラフをクリア
    plt.close()
