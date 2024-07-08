import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from scipy import stats

# CSVファイルを読み込む
data = pd.read_csv('alpha_slope.csv', header=None, names=['alpha', 'slope'])

# 1.3 <= alpha <= 1.8 の範囲のデータを抽出
mask = (data['alpha'] >= 1.3) & (data['alpha'] <= 1.8)
filtered_data = data[mask]

# 線形回帰を行う
slope, intercept, r_value, p_value, std_err = stats.linregress(filtered_data['alpha'], filtered_data['slope'])

# グラフを作成
plt.figure(figsize=(10, 6))

# 全データをプロット
plt.scatter(data['alpha'], data['slope'], color='blue', label='データ')

# フィッティング線を描画（横軸の範囲を拡張）
x_fit = np.linspace(1.1, 2.0, 100)
y_fit = slope * x_fit + intercept
plt.plot(x_fit, y_fit, color='red', label=f'y = {slope:.2f}x + {intercept:.2f}')

# alpha > 2 の範囲で y = 0.5 の直線を描画
x_line = np.linspace(2, data['alpha'].max(), 100)
y_line = np.full_like(x_line, 0.5)
plt.plot(x_line, y_line, color='green', linestyle='--', label='y = 0.5')

plt.xlabel('Alpha')
plt.ylabel('Slope')
plt.title('Alpha vs Slope')
plt.legend()
plt.grid(True)

# グラフの軸範囲を設定
plt.xlim(0.0, plt.xlim()[1])  
plt.ylim(0.4, 1.1)  # y軸の最大値を1.1に設定

# グラフを保存
plt.savefig('graph_alpha_slope.png')
plt.close()

print('グラフが graph_alpha_slope.png として保存されました。')
