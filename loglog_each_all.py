import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import os
import glob

# 保存ディレクトリを作成
output_dir = "loglog_each_all"
os.makedirs(output_dir, exist_ok=True)

for alpha_tenth in range(11, 20):  # 1.1 から 1.9 まで 0.1 刻み
    alpha = alpha_tenth / 10
    
    plt.figure(figsize=(10, 6))  # Figureをループ内で初期化

    csv_files = glob.glob(f'F_all/alpha-{alpha:.1f}/*.csv')
    
    for csv_file in csv_files:
        df = pd.read_csv(csv_file, header=None)
        l = df.iloc[:, 0].values
        F = df.iloc[:, 1].values

        plt.loglog(l, F, linestyle='-')  # ファイル名をラベルにしない

    plt.title(f"Log-log graph (α = {alpha:.1f})")
    plt.xlabel("l")
    plt.ylabel("F")
    plt.grid(True)
    # plt.legend() # 凡例を非表示

    # グラフを保存
    save_path = os.path.join(output_dir, f"loglog_graph_alpha-{alpha:.1f}.png")
    plt.savefig(save_path)
    plt.close() # 各alphaごとにFigureを閉じる

    print(f"グラフが {save_path} として保存されました。")