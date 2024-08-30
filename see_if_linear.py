"""alpha = 1.1~2.0において、l の値が10^4~10^7の範囲で本当に直線なのか調べる。
そのために拡大した、見やすいグラフを作成する。"""


import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import glob


# beki_walk_alpha_*.csvファイルを取得
#csv_files = glob.glob("l_F_beki_walk_alpha_*.csv")
csv_files = sorted(glob.glob("F_good/*.csv"))

# FigureとAxesオブジェクトを作成
fig, ax = plt.subplots(figsize=(10, 6))

# 色のリストを作成
colors = ['blue', 'green', 'red', 'cyan', 'magenta', 'yellow', 'black']

# マーカーの種類を追加
markers = ['o', 's', '^', 'v', '<', '>', 'D']

# ファイルごとに処理
for i, csv_file in enumerate(csv_files):
    # "2."を含むファイルをスキップ
    if "2." in csv_file and not "2.0" in csv_file and not "0.2" in csv_file and not "1.2" in csv_file:
        continue
        # "2."を含むファイルをスキップ
    if "3." in csv_file and not "0.3" in csv_file and not "1.3" in csv_file:
        continue
    # CSVファイルを読み込む
    input_file = csv_file
    df = pd.read_csv(input_file)

    # lとFの値を取得
    l = df.iloc[:, 0].values
    F = df.iloc[:, 1].values

    # alpha = 1.1~2.0のデータだけを使う
    if 1.1 <= float(csv_file[7:10]) <= 2.0:
        # l の値が10^4~10^7の範囲を拡大
        l_range = (l >= 10**4) & (l <= 10**7)
        l = l[l_range]
        F = F[l_range]

        # 両対数グラフにプロット（散布図として）
        ax.loglog(l, F, marker=markers[i % len(markers)], linestyle='', label=f'Data_{csv_file}', color=colors[i % len(colors)], markersize=3)

        # 直線フィッティング（対数変換したデータで行う）
        log_l = np.log(l)
        log_F = np.log(F)
        coefficients = np.polyfit(log_l, log_F, 1)
        slope = coefficients[0]
        intercept = coefficients[1]
        print(f"{csv_file}: slope={slope:.2f}, intercept={intercept:.2f}")

        # フィッティング線をプロット（元のスケールで）
        #fit_line = np.exp(intercept) * l ** slope
        #ax.loglog(l, fit_line, linestyle='--', color=colors[i % len(colors)], label=f'Fit Line_{csv_file}')

        # フィッティングの式をグラフに表示
        #ax.text(0.1, 0.8, f'F = {np.exp(intercept):.2f} * l^{slope:.2f}', transform=plt.gca().transAxes)

# グラフのタイトル、軸ラベル、凡例を設定
plt.title(f"Log-log graph and linear fitting (alpha=1.1~2.0, l=10^4~10^7)", fontsize=18)
plt.xlabel("l", fontsize=16)
plt.ylabel("F", fontsize=16)
plt.legend(fontsize=14)

# 横軸の範囲を設定
#plt.xlim([1.0, max(l)])
# グラフのタイトルを日本語に変更
plt.title("loglog")


# 凡例の位置を調整
#plt.legend(loc='lower right')

# x軸とy軸のラベルを日本語に変更
plt.xlabel(" l ")
plt.ylabel(" F ")

# 縦軸の範囲を設定
plt.ylim([10**1, 10**4.5]) # 例：0.1から100まで

# グリッドを表示
plt.grid(True)

# グラフを保存
plt.savefig(f"graph_all_zoom.png")

# グラフをクリア
plt.close()