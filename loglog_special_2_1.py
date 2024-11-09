import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import os
import glob


def calcualte_F_calc_alpha_above2(l, alpha):
    global coffi_alpha_above2
    mu = alpha / (alpha - 1)
    
    c1 = (1+2*alpha-alpha**2)/(2*alpha*(alpha-2))

    coffi_alpha_above2 =  (1+2*c1 )/ (15 * mu)
    #coffi_alpha_above2 =  (1+c1 )/ (15 * mu)
    #coffi_alpha_above2 = 1/(15*mu)
    F2 =  (
        coffi_alpha_above2*l
    )
    F = np.sqrt(F2)
    return F



# 保存ディレクトリを作成
output_dir = "loglog_each_all"
os.makedirs(output_dir, exist_ok=True)


alpha = 2.1

plt.figure(figsize=(10, 6))  # Figureをループ内で初期化

csv_files = glob.glob(f'F_all/alpha-{alpha:.1f}/*.csv')

# マーカーの種類を追加
markers = ['o', 's', '^', 'v', '<', '>', 'D']

for i, csv_file in enumerate(csv_files):
    df = pd.read_csv(csv_file, header=None)
    l = df.iloc[:, 0].values
    F = df.iloc[:, 1].values
    
    # l >= 10^4 のデータのみを使用
    mask = l >= 1e4
    l = l[mask]
    F = F[mask]

    plt.loglog(l, F, linestyle='', marker=markers[i % len(markers)], markersize=10, label=os.path.basename(csv_file))  # マーカーの形を変更

l_calc = np.logspace(4, np.log10(l.max()), 100)  # 10^4 から最大の l まで
F_calc_above2 = calcualte_F_calc_alpha_above2(l_calc, alpha)
plt.loglog(l_calc, F_calc_above2, linestyle='-', color='red', linewidth=1.5,
            label=f'Calc (α > 2): F = {coffi_alpha_above2:.3f}*l^0.5')

plt.title(f"Log-log graph (α = {alpha:.1f})")
plt.xlabel("l")
plt.ylabel("F")
plt.grid(True)
plt.legend()  # 凡例を表示
plt.xlim(left=1e4)  # x軸の最小値を10^4に設定

# グラフを保存
save_path = os.path.join(output_dir, f"loglog_graph_alpha-{alpha:.1f}.png")
plt.savefig(save_path)
plt.close() # 各alphaごとにFigureを閉じる

print(f"グラフが {save_path} として保存されました。")